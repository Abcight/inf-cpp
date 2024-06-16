#pragma once

/// <summary>
/// A singular command issued to the renderer during userland script execution.
/// </summary>
class RenderCommand {
private:
	glm::vec2 position;
	glm::vec2 scale;
	float rotation;
	float layer;
	Framebuffer* target = nullptr;
	Bindable** bindables;
	int bindables_length = 0;
public:
	RenderCommand();
	static void export_type(sol::state& target);
	static bool ord_layer(RenderCommand& a, RenderCommand& b);
	RenderCommand& to(Framebuffer* buffer);
	RenderCommand& with(Bindable* bindable);
	RenderCommand& with_position(glm::vec2 position);
	RenderCommand& with_position_xy(float x, float y);
	RenderCommand& with_scale(glm::vec2 scale);
	RenderCommand& with_scale_xy(float x, float y);
	RenderCommand& with_rotation(float rotation);
	RenderCommand& with_layer(float layer);
	friend class Renderer;
};