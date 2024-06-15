G_TEXTURE_WALL = Texture.open("wall.jpg"):unwrap();
G_TEXTURE_DOGE = Texture.open("doge.jpg"):unwrap();
G_SOUND_BARK = Audio.open("dog.ogg"):unwrap();
G_FRAME = Framebuffer.create(Renderer.screen_width, Renderer.screen_height):unwrap();

local p_pos = Vec2.new();

function Update()
	G_FRAME:clear();

	local targets = { G_TEXTURE_WALL, G_TEXTURE_DOGE };
	for i, target in pairs(targets) do
		local x = Renderer.screen_width / 2;
		local y = Renderer.screen_height / 2;
		x = x + math.sin(os.clock() + i) * Renderer.screen_width / 3;

		Renderer:queue_command(
			RenderCommand
				:new()
				:to(G_FRAME)
				:with(target)
				:with_scale(100, 100)
				:with_position(x, y)
		);
	end

	p_pos = Vec2.lerp(
		Input:get_cursor_position(),
		p_pos,
		math.pow(0.0001, G_DT)
	);

	Renderer:queue_command(
		RenderCommand:new()
			:with(G_FRAME.texture)
			:with(Color.RED)
			:with_scale(100, 100)
			:with_position(p_pos)
			:with_layer(5)
	);

	if Input:get_key_down(' ') then
		G_SOUND_BARK:play();
	end
end