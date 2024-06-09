G_TEXTURE_WALL = Texture.open("wall.jpg"):unwrap();
G_TEXTURE_DOGE = Texture.open("doge.jpg"):unwrap();

function Update()
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