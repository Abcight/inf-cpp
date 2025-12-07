#include <algorithm>
#include <iostream>
#include <glm/glm.hpp>

#include "renderer.h"
#include "framebuffer.h"

RenderCommand::RenderCommand() {
	this->position = glm::vec2(0.0f, 0.0f);
	this->scale = glm::vec2(1.0f, 1.0f);
	this->rotation = 0.0f;
	this->layer = 1.0f;
	this->bindables = new Bindable * [10];
}

RenderCommand& RenderCommand::to(Framebuffer* buffer) {
	this->target = buffer;
	return *this;
}

RenderCommand& RenderCommand::with(Bindable* bindable) {
	this->bindables[bindables_length] = bindable;
	bindables_length++;
	return *this;
}

RenderCommand& RenderCommand::with_position(glm::vec2 position) {
	this->position = position;
	return *this;
}

RenderCommand& RenderCommand::with_position_xy(float x, float y) {
	this->position.x = x;
	this->position.y = y;
	return *this;
}

RenderCommand& RenderCommand::with_scale(glm::vec2 scale) {
	this->scale = scale;
	return *this;
}

RenderCommand& RenderCommand::with_scale_xy(float x, float y) {
	this->scale.x = x;
	this->scale.y = y;
	return *this;
}

RenderCommand& RenderCommand::with_rotation(float rotation) {
	this->rotation = rotation;
	return *this;
}

RenderCommand& RenderCommand::with_layer(float layer) {
	this->layer = layer;
	return *this;
}

bool RenderCommand::ord_layer(RenderCommand& a, RenderCommand& b) {
	return a.layer > b.layer;
}

void RenderCommand::export_type(sol::state& target) {
	target.new_usertype<RenderCommand>(
		"RenderCommand",
		"position", &RenderCommand::position,
		"scale", &RenderCommand::scale,
		"rotation", &RenderCommand::rotation,
		"layer", &RenderCommand::layer,
		"to", &RenderCommand::to,
		"with", &RenderCommand::with,
		"with_position", sol::overload(
			&RenderCommand::with_position,
			&RenderCommand::with_position_xy
		),
		"with_scale", sol::overload(
			&RenderCommand::with_scale,
			&RenderCommand::with_scale_xy
		),
		"with_rotation", &RenderCommand::with_rotation,
		"with_layer", &RenderCommand::with_layer
	);
}