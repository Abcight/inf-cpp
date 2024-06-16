math.randomseed(os.time())

local SQUARE = Texture.open("square.png"):unwrap()
local CHIME = Audio.open("chime.ogg"):unwrap()
local BUZZER = Audio.open("buzzer.ogg"):unwrap()
local DIR = {LEFT=0, RIGHT=1, UP=2, DOWN=3}

function reset_game()
	timer = 0
	direction = DIR.DOWN
	snake = { head = {x = 5, y = 5, next = nil} }
	fruit = { x = 3, y = 3 }
end

reset_game()

function draw_cell(x, y, color)
	local unit_x = Renderer.screen_width / 10
	local unit_y = Renderer.screen_height / 10

	Renderer:queue_command(
		RenderCommand:
			new()
			:with(color)
			:with_position(unit_x * x, unit_y * y)
			:with_scale(unit_x, unit_y)
	)
end

function draw(head)
	draw_cell(head.x, head.y, Color.WHITE);
	if head.next ~= nil then
		draw(head.next)
	end
end

function remove_tail(head)
	if head.next.next == nil then head.next = nil
	else remove_tail(head.next) end
end

function in_tail(head, x, y)
	if head.x == x and head.y == y then return true end
	if head.next ~= nil then
		return in_tail(head.next, x, y)
	end
end

function Update()
	timer = timer + G_DT

	draw_cell(fruit.x, fruit.y, Color.RED)

	if Input:get_key('W') then direction = DIR.UP end
	if Input:get_key('S') then direction = DIR.DOWN end
	if Input:get_key('A') then direction = DIR.LEFT end
	if Input:get_key('D') then direction = DIR.RIGHT end

	if timer > 0.2 then
		timer = 0
		local new_head = { x=snake.head.x,y=snake.head.y,next=snake.head }
		if direction == DIR.LEFT then new_head.x = new_head.x - 1 end
		if direction == DIR.RIGHT then new_head.x = new_head.x + 1 end
		if direction == DIR.UP then new_head.y = new_head.y + 1 end
		if direction == DIR.DOWN then new_head.y = new_head.y - 1 end
		if new_head.x < 0 then new_head.x = 10 end
		if new_head.x > 10 then new_head.x = 0 end
		if new_head.y < 0 then new_head.y = 10 end
		if new_head.y > 10 then new_head.y = 0 end

		if in_tail(snake.head, new_head.x, new_head.y) then
			BUZZER:play()
			reset_game()
			return
		end

		snake.head = new_head

		if new_head.x == fruit.x and new_head.y == fruit.y then
			CHIME:play()
			while in_tail(snake.head, fruit.x, fruit.y) do
				fruit.x = math.random(1, 9)
				fruit.y = math.random(1, 9)
			end
		else
			remove_tail(snake.head)
		end
	end

	draw(snake.head)
end