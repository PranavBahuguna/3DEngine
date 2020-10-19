#include "DrawListDecorator.h"

// Set the input draw list as being owned by the decorator
DrawListDecorator::DrawListDecorator(DrawListUptr drawList) : m_drawList(std::move(drawList)) {}

// For each decorator method, delegate the work to the owned draw list
ShaderSptr DrawListDecorator::getShader() { return m_drawList->getShader(); }

DrawTargets &DrawListDecorator::getTargets() { return m_drawList->getTargets(); }

void DrawListDecorator::draw(ERROR &errCode) { m_drawList->draw(errCode); }

void DrawListDecorator::addTarget(const DrawSptr &target) { m_drawList->addTarget(target); }

void DrawListDecorator::addTargets(const DrawTargets &targets) { m_drawList->addTargets(targets); }

void DrawListDecorator::setShader(ShaderSptr shader) { m_drawList->setShader(shader); }