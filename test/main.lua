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

	local p_command = RenderCommand.new();
	p_command.scale = Vec2.new(100, 100);
	p_command.position = p_pos;
	p_command.texture = G_TEXTURE_DOGE;
	Renderer:queue_command(p_command);

	if Input:get_key_down(' ') then
		G_SOUND_BARK:play();
	end

	local targets = {G_TEXTURE_WALL, G_TEXTURE_DOGE};
	for i, target in pairs(targets) do
		local command = RenderCommand.new();
		command.scale.x = 100;
		command.scale.y = 100;

		command.position.x = Renderer.screen_width / 2;
		command.position.y = Renderer.screen_height / 2;
		command.texture = target;

		command.position.x =
			command.position.x +
			math.sin(os.clock() + i) * Renderer.screen_width / 3;

		Renderer:queue_command(command);
	end
end