#pragma once

#include "DrawList.h"

class TransparencyDrawList : public DrawListDecorator {
public:
  TransparencyDrawList(DListPtr drawList);

  void draw(ERROR &errCode) override;
};