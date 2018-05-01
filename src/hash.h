// Copyright 1992, 1999, 2011-2015, 2017-2018 by Jon Dart.  All Rights Reserved.

#ifndef _HASH_H
#define _HASH_H

#include "chess.h"
#include "board.h"
#ifdef _DEBUG
#include "legal.h"
#endif
#include <climits>
#include <cstddef>

extern const hash_t rep_codes[3];

class HashEntry {

	public:

      // Contents of the flag field
      enum {
         TYPE_MASK = 0x7,
         TB_MASK = 0x08,
         LEARNED_MASK = 0x10
      };

      static const int QSEARCH_CHECK_DEPTH = -1;
      static const int QSEARCH_NO_CHECK_DEPTH = -2;

      // Only the first 4 values are actually stored - Invalid indicates
      // a hash hit with inadequate depth; NoHit indicates failure to find
      // a hash match.
      enum ValueType { Valid, UpperBound, LowerBound, Eval, Invalid, NoHit };

    HashEntry() {
          contents.depth = 0;
          contents.age = 0;
          contents.flags = Eval;
          contents.start = contents.dest = InvalidSquare;
          contents.promotion = Empty;
          contents.start = InvalidSquare;
          contents.value = Constants::INVALID_SCORE;
          setEffectiveHash(0,Constants::INVALID_SCORE);
      }

      HashEntry(hash_t hash, score_t val, score_t staticValue, int depth,
         ValueType type, unsigned age, byte flags = 0,
                Move bestMove = NullMove) {
         ASSERT(depth+2 >= 0 && depth+2 < 256);
         contents.depth = (byte)(depth+2);
         contents.age = age;
         contents.flags = type | flags;
         contents.start = StartSquare(bestMove);
         contents.dest = DestSquare(bestMove);
         contents.promotion = PromoteTo(bestMove);
         contents.value = stored_score_t(val);
         setEffectiveHash(hash,staticValue);
      }

      int empty() const {
         return getEffectiveHash() == 0x0ULL;
      }

      void clear() {
        hc = 0x0ULL;
      }

      int depth() const {
         return (int)contents.depth - 2;
      }

      score_t getValue() const {
         return score_t(contents.value);
      }

      score_t staticValue() const {
          // assumes 2's complement machine
          unsigned bits = unsigned(hc & STATIC_VALUE_MASK);
          if (bits & 0x8000)
              return score_t(-32768 + (bits & 0x7fff));
          else
              return score_t(bits & 0x7fff);
      }

      void setValue(score_t val) {
         contents.value = (stored_score_t)val;
      }

      ValueType type() const {
         return (ValueType)(contents.flags & TYPE_MASK);
      }

      unsigned age() const {
         return (int)contents.age;
      }

      void setAge(unsigned age) {
         contents.age = age;
      }

      int learned() const {
         return (int)((contents.flags & LEARNED_MASK) != 0);
      }

      int tb() const {
         return (int)((contents.flags & TB_MASK) != 0);
      }

      byte flags() const {
          return contents.flags;
      }

      Move bestMove(const Board &b) const {
         if (contents.start == InvalidSquare)
            return NullMove;
         else {
            return CreateMove(b,(Square)contents.start,(Square)contents.dest,
               (PieceType)contents.promotion);
         }
      }

      bool avoidNull(int null_depth, score_t beta) const {
         return type() == UpperBound && depth() >= null_depth &&
              getValue() < beta;
      }

      int operator == (const hash_t hash) const {
         return getEffectiveHash() == (hash & HASH_MASK);
      }

      int operator != (const hash_t hash) const {
         return getEffectiveHash() != (hash & HASH_MASK);
      }

      hash_t getEffectiveHash() const {
         return (hc ^ val) & HASH_MASK;
      }

      void setEffectiveHash(hash_t hash, score_t static_score) {
         hc = ((hash ^ val) & HASH_MASK) | (uint16_t)static_score;
      }

   protected:

      static const hash_t HASH_MASK = 0xffffffffffff0000;
      static const hash_t STATIC_VALUE_MASK = ~HASH_MASK;

#ifdef __INTEL_COMPILER
#pragma pack(push,1)
#endif
      struct Contents
      BEGIN_PACKED_STRUCT
        stored_score_t value;
        byte depth;
        byte age;
        byte flags;
        signed char start, dest, promotion;
      END_PACKED_STRUCT

#ifdef __INTEL_COMPILER
#pragma pack(pop)
#endif
      union
      {
          Contents contents;
          uint64_t val;
      };

      // static value is packed in the low-order hash bits
      uint64_t hc;
};

class Hash {

  friend class Scoring;
 public:
    Hash();

    void initHash(size_t bytes);

    void resizeHash(size_t bytes);

    void freeHash();

    void clearHash();

    // put info from the external permanent hash table into the
    // in-memory hash table
    void loadLearnInfo();

    HashEntry::ValueType searchHash(const Board& b,hash_t hashCode,
                                              int ply,
                                              int depth, unsigned age,
                                              HashEntry &he
                                              ) {
        if (!hashSize) return HashEntry::NoHit;
        int probe = (int)(hashCode & hashMask);

        HashEntry *p = &hashTable[probe];
        HashEntry *hit = nullptr;
        for (int i = MaxRehash; i != 0; --i, p++) {
            // Copy hashtable entry before hash test below (avoids
            // race where entry is validated, then changed).
            HashEntry entry(*p);
            if (entry == hashCode) {
                // we got a hit on this entry in the current search,
                // so update the age to discourage replacement:
                if (entry.age() && (entry.age() != age)) {
                   entry.setAge(age);
                   entry.setEffectiveHash(hashCode,entry.staticValue());
                   *p = entry;
                }
                hit = p;
                he = entry;
                if (entry.depth() >= depth) {
                    // usable depth
                    return he.type();
                }
                else {
                    break;
                }
            }
        }
        if (hit) {
          // hash hit, but with insufficient depth:
          return HashEntry::Invalid;
        } else {
          return HashEntry::NoHit;
        }
    }

    void storeHash(hash_t hashCode, const int depth,
                          unsigned age,
                          HashEntry::ValueType type,
                          score_t value,
                          score_t staticValue,
                          int flags,
                          Move best_move) {

        if (!hashSize) return;
        int probe = (int)(hashCode & hashMask);
        HashEntry *p = &hashTable[probe];

        HashEntry *best = nullptr;
        ASSERT(value >= SHRT_MIN && value <= SHRT_MAX);
        // Of the positions that hash to the same locations
        // as this one, find the best one to replace.
        score_t maxScore = score_t(-Constants::MaxPly*DEPTH_INCREMENT);
        for (int i = MaxRehash; i != 0; --i) {
            HashEntry &q = *p;

            if (q.empty()) {
                // empty hash entry, available for use
                best = &q;
                break;
            }
            else if (q == hashCode) {
                // same position, always replace
                best = &q;
                break;
            }
            else if (!(q.flags() &
                       (HashEntry::LEARNED_MASK | HashEntry::TB_MASK)) &&
                     (q.depth() <= depth || q.age() != age)) {
                // candidate for replacement
                score_t score = replaceScore(q,age);
                if (score > maxScore) {
                    maxScore = score;
                    best = &q;
                }
            }
            p++;
        }
        if (best != nullptr) {
            if (best->empty()) {
               hashFree--;
            }
            *best = HashEntry(hashCode, value,
                              staticValue, depth, type, age, flags, best_move);
       }
    }

    size_t getHashSize() const {
        return hashSize;
    }

    // Percent full (percentage x 10)
    int pctFull() const {
        if (hashSize == 0)
            return 0;
        else
            return (int)(1000.0*(hashSize-hashFree)/(double)hashSize);
    }

private:
    score_t replaceScore(const HashEntry &pos, int age) const {
        return score_t((std::abs((int)pos.age()-(int)age)<<12) - pos.depth());
    }

    HashEntry *hashTable;
    size_t hashSize, hashFree;
    hash_t hashMask;
    static const int MaxRehash = 4;
    int hash_init_done;
};

#endif
