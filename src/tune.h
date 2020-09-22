// Copyright 2016-2019 by Jon Dart. All Rights Reserved.

#ifndef _TUNE_H
#define _TUNE_H

#include "scoring.h"

#include <string>
#include <vector>

struct TuneParam {
   int index;
   string name;
   score_t current;
   score_t min_value;
   score_t max_value;
   enum Scaling {None, Midgame, Endgame, Any};
   Scaling scaling;
   int tunable;

   TuneParam(int i, const string &n, score_t c, score_t minv, score_t maxv, Scaling s = None,int t = 0) :
      index(i),name(n),current(c),min_value(minv),max_value(maxv),scaling(s),tunable(t) {
   }
   TuneParam():
      index(-1), name(""), current(0), min_value(0), max_value(0),scaling(None),tunable(0) {
   }
   score_t range() const {
      return max_value - min_value;
   }
   score_t scaled() const {
      return (current - min_value)/range();
   };
   void scale(score_t val) {
      current = val*range() + min_value;
   }
   void scale() {
      current = current*range() + min_value;
   }
};

class Tune : public vector<TuneParam> {

 public:

  Tune();

  virtual ~Tune() {
  }

  enum {
    KN_VS_PAWN_ADJUST0,
    KN_VS_PAWN_ADJUST1,
    KN_VS_PAWN_ADJUST2,
    KING_ATTACK_SCALE_MAX,
    KING_ATTACK_SCALE_INFLECT,
    KING_ATTACK_SCALE_FACTOR,
    KING_ATTACK_SCALE_BIAS,
    KING_COVER1,
    KING_COVER2,
    KING_COVER3,
    KING_COVER4,
    KING_FILE_HALF_OPEN,
    KING_FILE_OPEN,
    KING_COVER_FILE_FACTOR0,
    KING_COVER_FILE_FACTOR1,
    KING_COVER_FILE_FACTOR2,
    KING_COVER_FILE_FACTOR3,
    KING_COVER_BASE,
    KING_DISTANCE_BASIS,
    KING_DISTANCE_MULT,
    PIN_MULTIPLIER_MID,
    PIN_MULTIPLIER_END,
    MINOR_FOR_PAWNS_MIDGAME,
    MINOR_FOR_PAWNS_ENDGAME,
    RB_ADJUST_MIDGAME,
    RB_ADJUST_ENDGAME,
    RBN_ADJUST_MIDGAME,
    RBN_ADJUST_ENDGAME,
    QR_ADJUST_MIDGAME,
    QR_ADJUST_ENDGAME,
    Q_VS_3MINORS_MIDGAME,
    Q_VS_3MINORS_ENDGAME,
    KRMINOR_VS_R_NO_PAWNS,
    KQMINOR_VS_Q_NO_PAWNS,
    TRADE_DOWN1,
    TRADE_DOWN2,
    TRADE_DOWN3,
    PAWN_ENDGAME_ADJUST,
    PAWN_ATTACK_FACTOR,
    MINOR_ATTACK_FACTOR,
    MINOR_ATTACK_BOOST,
    ROOK_ATTACK_FACTOR,
    ROOK_ATTACK_BOOST,
    QUEEN_ATTACK_FACTOR,
    QUEEN_ATTACK_BOOST,
    KING_ATTACK_COVER_BOOST_BASE,
    KING_ATTACK_COVER_BOOST_SLOPE,
    KING_ATTACK_COUNT,
    KING_ATTACK_SQUARES,
    OWN_PIECE_KING_PROXIMITY_MIN,
    OWN_MINOR_KING_PROXIMITY,
    OWN_ROOK_KING_PROXIMITY,
    OWN_QUEEN_KING_PROXIMITY,
    PAWN_PUSH_THREAT_MID,
    PAWN_PUSH_THREAT_END,
    ENDGAME_KING_THREAT,
    BISHOP_TRAPPED,
    BISHOP_PAIR_MID,
    BISHOP_PAIR_END,
    BISHOP_PAWN_PLACEMENT_END,
    BAD_BISHOP_MID,
    BAD_BISHOP_END,
    CENTER_PAWN_BLOCK,
    OUTSIDE_PASSER_MID,
    OUTSIDE_PASSER_END,
    WEAK_PAWN_MID,
    WEAK_PAWN_END,
    WEAK_ON_OPEN_FILE_MID,
    WEAK_ON_OPEN_FILE_END,
    SPACE,
    PAWN_CENTER_SCORE_MID,
    ROOK_ON_7TH_MID,
    ROOK_ON_7TH_END,
    TWO_ROOKS_ON_7TH_MID,
    TWO_ROOKS_ON_7TH_END,
    TRAPPED_ROOK_NO_CASTLE,
    ROOK_ON_OPEN_FILE_MID,
    ROOK_ON_OPEN_FILE_END,
    ROOK_BEHIND_PP_MID,
    ROOK_BEHIND_PP_END,
    QUEEN_OUT,
    PAWN_SIDE_BONUS,
    KING_OWN_PAWN_DISTANCE,
    KING_OPP_PAWN_DISTANCE,
    SIDE_PROTECTED_PAWN,
    KING_POSITION_LOW_MATERIAL0,
    KING_POSITION_LOW_MATERIAL1,
    KING_POSITION_LOW_MATERIAL2,
    PASSED_PAWN_MID2,
    PASSED_PAWN_MID3,
    PASSED_PAWN_MID4,
    PASSED_PAWN_MID5,
    PASSED_PAWN_MID6,
    PASSED_PAWN_MID7,
    PASSED_PAWN_END2,
    PASSED_PAWN_END3,
    PASSED_PAWN_END4,
    PASSED_PAWN_END5,
    PASSED_PAWN_END6,
    PASSED_PAWN_END7,
    PASSED_PAWN_FILE_ADJUST1,
    PASSED_PAWN_FILE_ADJUST2,
    PASSED_PAWN_FILE_ADJUST3,
    PASSED_PAWN_FILE_ADJUST4,
    CONNECTED_PASSER_MID2,
    CONNECTED_PASSER_MID3,
    CONNECTED_PASSER_MID4,
    CONNECTED_PASSER_MID5,
    CONNECTED_PASSER_MID6,
    CONNECTED_PASSER_MID7,
    CONNECTED_PASSER_END2,
    CONNECTED_PASSER_END3,
    CONNECTED_PASSER_END4,
    CONNECTED_PASSER_END5,
    CONNECTED_PASSER_END6,
    CONNECTED_PASSER_END7,
    ADJACENT_PASSER_MID2,
    ADJACENT_PASSER_MID3,
    ADJACENT_PASSER_MID4,
    ADJACENT_PASSER_MID5,
    ADJACENT_PASSER_MID6,
    ADJACENT_PASSER_MID7,
    ADJACENT_PASSER_END2,
    ADJACENT_PASSER_END3,
    ADJACENT_PASSER_END4,
    ADJACENT_PASSER_END5,
    ADJACENT_PASSER_END6,
    ADJACENT_PASSER_END7,
    QUEENING_PATH_CLEAR_MID2,
    QUEENING_PATH_CLEAR_MID3,
    QUEENING_PATH_CLEAR_MID4,
    QUEENING_PATH_CLEAR_MID5,
    QUEENING_PATH_CLEAR_MID6,
    QUEENING_PATH_CLEAR_MID7,
    QUEENING_PATH_CLEAR_END2,
    QUEENING_PATH_CLEAR_END3,
    QUEENING_PATH_CLEAR_END4,
    QUEENING_PATH_CLEAR_END5,
    QUEENING_PATH_CLEAR_END6,
    QUEENING_PATH_CLEAR_END7,
    PP_OWN_PIECE_BLOCK_MID5,
    PP_OWN_PIECE_BLOCK_MID6,
    PP_OWN_PIECE_BLOCK_MID7,
    PP_OWN_PIECE_BLOCK_END5,
    PP_OWN_PIECE_BLOCK_END6,
    PP_OWN_PIECE_BLOCK_END7,
    PP_OPP_PIECE_BLOCK_MID5,
    PP_OPP_PIECE_BLOCK_MID6,
    PP_OPP_PIECE_BLOCK_MID7,
    PP_OPP_PIECE_BLOCK_END5,
    PP_OPP_PIECE_BLOCK_END6,
    PP_OPP_PIECE_BLOCK_END7,
    QUEENING_PATH_CONTROL_MID5,
    QUEENING_PATH_CONTROL_MID6,
    QUEENING_PATH_CONTROL_MID7,
    QUEENING_PATH_CONTROL_END5,
    QUEENING_PATH_CONTROL_END6,
    QUEENING_PATH_CONTROL_END7,
    QUEENING_PATH_OPP_CONTROL_MID5,
    QUEENING_PATH_OPP_CONTROL_MID6,
    QUEENING_PATH_OPP_CONTROL_MID7,
    QUEENING_PATH_OPP_CONTROL_END5,
    QUEENING_PATH_OPP_CONTROL_END6,
    QUEENING_PATH_OPP_CONTROL_END7,
    DOUBLED_PAWNS_MID1,
    DOUBLED_PAWNS_MID2,
    DOUBLED_PAWNS_MID3,
    DOUBLED_PAWNS_MID4,
    DOUBLED_PAWNS_END1,
    DOUBLED_PAWNS_END2,
    DOUBLED_PAWNS_END3,
    DOUBLED_PAWNS_END4,
    TRIPLED_PAWNS_MID1,
    TRIPLED_PAWNS_MID2,
    TRIPLED_PAWNS_MID3,
    TRIPLED_PAWNS_MID4,
    TRIPLED_PAWNS_END1,
    TRIPLED_PAWNS_END2,
    TRIPLED_PAWNS_END3,
    TRIPLED_PAWNS_END4,
    ISOLATED_PAWN_MID1,
    ISOLATED_PAWN_MID2,
    ISOLATED_PAWN_MID3,
    ISOLATED_PAWN_MID4,
    ISOLATED_PAWN_END1,
    ISOLATED_PAWN_END2,
    ISOLATED_PAWN_END3,
    ISOLATED_PAWN_END4,

    KING_OPP_PASSER_DISTANCE = ISOLATED_PAWN_END4+1,
    OWN_PIECE_KING_PROXIMITY_MULT = KING_OPP_PASSER_DISTANCE+6,
    KNIGHT_PST_MIDGAME = OWN_PIECE_KING_PROXIMITY_MULT+16,
    KNIGHT_PST_ENDGAME = KNIGHT_PST_MIDGAME+32,
    BISHOP_PST_MIDGAME = KNIGHT_PST_ENDGAME+32,
    BISHOP_PST_ENDGAME = BISHOP_PST_MIDGAME+32,
    ROOK_PST_MIDGAME = BISHOP_PST_ENDGAME+32,
    ROOK_PST_ENDGAME = ROOK_PST_MIDGAME+32,
    QUEEN_PST_MIDGAME = ROOK_PST_ENDGAME+32,
    QUEEN_PST_ENDGAME = QUEEN_PST_MIDGAME+32,
    KING_PST_MIDGAME = QUEEN_PST_ENDGAME+32,
    KING_PST_ENDGAME = KING_PST_MIDGAME+32,
    THREAT_BY_PAWN = KING_PST_ENDGAME+32,
    THREAT_BY_KNIGHT = THREAT_BY_PAWN+10,
    THREAT_BY_BISHOP = THREAT_BY_KNIGHT+10,
    THREAT_BY_ROOK = THREAT_BY_BISHOP+10,
    KNIGHT_MOBILITY = THREAT_BY_ROOK+10,
    BISHOP_MOBILITY = KNIGHT_MOBILITY+9,
    ROOK_MOBILITY_MIDGAME = BISHOP_MOBILITY+15,
    ROOK_MOBILITY_ENDGAME = ROOK_MOBILITY_MIDGAME+15,
    QUEEN_MOBILITY_MIDGAME = ROOK_MOBILITY_ENDGAME+15,
    QUEEN_MOBILITY_ENDGAME = QUEEN_MOBILITY_MIDGAME+24,
    KING_MOBILITY_ENDGAME = QUEEN_MOBILITY_ENDGAME+24,
    KNIGHT_OUTPOST_MIDGAME = KING_MOBILITY_ENDGAME+5,
    KNIGHT_OUTPOST_ENDGAME = KNIGHT_OUTPOST_MIDGAME+2,
    BISHOP_OUTPOST_MIDGAME = KNIGHT_OUTPOST_ENDGAME+2,
    BISHOP_OUTPOST_ENDGAME = BISHOP_OUTPOST_MIDGAME+2,
    PAWN_STORM = BISHOP_OUTPOST_ENDGAME+2
  };
   
  int numTuningParams() const;

  // number of fixed parameters that map 1-1 to tuning paramaters
  int paramArraySize() const;

  // initialize scoring parameters from the current tuned parameter
  // set
  void applyParams(bool check = true) const;

  // check current param settings against bounds and verify indexes
  // in internal array
  void checkParams() const;

  // write parameter values to a stream
  void writeX0(ostream &);

  // read parameter values from a stream
  void readX0(istream &);

  int findParamByName(const string &name) const;

  static constexpr int NUM_MISC_PARAMS = KING_OPP_PASSER_DISTANCE-KN_VS_PAWN_ADJUST0;

  double scale(score_t value,int index,int materialLevel) const;

  score_t kingAttackSigmoid(score_t weight) const;

};
#endif
