#pragma once

#include "DrawListDecorator.h"

class TransparencyDrawList : public DrawListDecorator {
public:
  TransparencyDrawList(DrawListUptr drawList);

  void draw(ERROR &errCode) override;
};