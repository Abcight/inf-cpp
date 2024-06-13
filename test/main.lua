G_TEXTURE_WALL = Texture.open("wall.jpg"):unwrap();
G_TEXTURE_DOGE = Texture.open("doge.jpg"):unwrap();
G_SOUND_BARK = Audio.open("dog.ogg"):unwrap();

-- G_DT: Frame delta in seconds

local p_pos = Vec2.new();

function Update()
	p_pos = Vec2.lerp(
		Input:get_cursor_position(),
		p_pos,
		math.pow(0.01, G_DT)
	);

	Renderer:queue_command(
		RenderCommand:new()
			:with(G_TEXTURE_DOGE)
			:with_scale(100, 100)
			:with_position(p_pos)
	);

	if Input:get_key_down(' ') then
		G_SOUND_BARK:play();
	end

	local targets = { G_TEXTURE_WALL, G_TEXTURE_DOGE };
	for i, target in pairs(targets) do
		local x = Renderer.screen_width / 2;
		local y = Renderer.screen_height / 2;
		x = x + math.sin(os.clock() + i) * Renderer.screen_width / 3;

		Renderer:queue_command(
			RenderCommand
				:new()
				:with(target)
				:with_scale(100, 100)
				:with_position(x, y)
		);
	end
end