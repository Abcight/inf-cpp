math.randomseed(os.time())

local PLAYER = Texture.open("square.png"):unwrap()
local PLATFORM_CAP = Texture.open("square_cap.png"):unwrap()
local PLATFORM_MID = Texture.open("square_mid.png"):unwrap()

local PLAYER_WIDTH = 40
local PLAYER_HEIGHT = 40
local MOVE_SPEED = 600
local AIR_CONTROL = 400
local JUMP_VELOCITY = 900
local GRAVITY = -2000
local FRICTION = 8
local COYOTE_TIME = 0.12

local player = { x = 0, y = 0, vx = 0, vy = 0, on_ground = false, coyote = 0 }
local platforms = {}
local next_platform_y = 0
local camera_y = 0
local pending_reset = false
local game_started = false
local scroll_speed = 0
local SCROLL_ACCEL = 18
local SCROLL_MAX = 130

local function irand(a, b)
	local lo = math.floor(a)
	local hi = math.floor(b)
	if hi < lo then hi = lo end
	local span = hi - lo
	if span <= 0 then return lo end
	return lo + math.floor(math.random() * (span + 1))
end

local function spawn_platform()
	local w = irand(90, 180)
	local h = 24
	local margin = 20
	local x = irand(margin + w / 2, Renderer.screen_width - margin - w / 2)
	local y = next_platform_y
	table.insert(platforms, { x = x, y = y, w = w, h = h })
	next_platform_y = next_platform_y + irand(50, 110)
end

function reset()
	platforms = {}
	table.insert(platforms, { x = Renderer.screen_width / 2, y = 20, w = Renderer.screen_width, h = 40 })
	player.x = Renderer.screen_width / 2
	player.y = 80
	player.vx = 0
	player.vy = 0
	player.on_ground = false
	player.coyote = 0
	camera_y = 0
	next_platform_y = 120
	game_started = false
	scroll_speed = 0
	while next_platform_y < (camera_y + Renderer.screen_height * 1.2) do
		spawn_platform()
	end
end

reset()

function Update()
	local dt = G_DT

	if pending_reset then
		pending_reset = false
		reset()
		return
	end

	-- Start auto-scroll on first jump input
	if not game_started and (Input:get_key_down(' ') or Input:get_key_down('W')) then
		game_started = true
	end

	local move = 0
	if Input:get_key('A') then move = move - 1 end
	if Input:get_key('D') then move = move + 1 end
	if player.on_ground then
		player.vx = player.vx + move * MOVE_SPEED * dt
		player.vx = player.vx * math.max(0, 1 - FRICTION * dt)
	else
		player.vx = player.vx + move * AIR_CONTROL * dt
	end

	if player.coyote > 0 and (Input:get_key_down(' ') or Input:get_key_down('W')) then
		player.vy = JUMP_VELOCITY
		player.on_ground = false
		player.coyote = 0
	end

	player.vy = player.vy + GRAVITY * dt

	local next_x = player.x + player.vx * dt
	local next_y = player.y + player.vy * dt

	local sw = Renderer.screen_width
	if next_x < 0 then next_x = sw end
	if next_x > sw then next_x = 0 end

	local landed = false
	if player.vy <= 0 then
		for i = 1, #platforms do
			local p = platforms[i]
			local top = p.y + p.h / 2
			local was_above = (player.y - PLAYER_HEIGHT / 2) >= top
			local will_below_or_touch = (next_y - PLAYER_HEIGHT / 2) <= top
			local horizontal_overlap = math.abs(next_x - p.x) * 2 < (PLAYER_WIDTH + p.w)
			if was_above and will_below_or_touch and horizontal_overlap then
				next_y = top + PLAYER_HEIGHT / 2
				player.vy = 0
				player.on_ground = true
				player.coyote = COYOTE_TIME
				landed = true
				break
			end
		end
	end

	if not landed then
		if player.on_ground and player.coyote <= 0 then
			player.coyote = COYOTE_TIME
		end
		player.on_ground = false
	end

	if player.coyote > 0 then
		player.coyote = player.coyote - dt
		if player.coyote < 0 then player.coyote = 0 end
	end

	player.x = next_x
	player.y = next_y

	-- Scroll once the game has started
	if game_started then
		scroll_speed = math.min(SCROLL_MAX, scroll_speed + SCROLL_ACCEL * dt)
		camera_y = camera_y + scroll_speed * dt
	end

	-- Camera follows player upward if they get ahead of scroll
	local follow_y = player.y - Renderer.screen_height * 0.35
	if follow_y > camera_y then
		camera_y = follow_y
	end

	if player.y < (camera_y - 60) then
		pending_reset = true
		return
	end

	while next_platform_y < camera_y + Renderer.screen_height * 1.5 do
		spawn_platform()
	end

	local keep = {}
	for i = 1, #platforms do
		local p = platforms[i]
		if p.y > camera_y - 200 then
			table.insert(keep, p)
		end
	end
	platforms = keep

	for i = 1, #platforms do
		local p = platforms[i]
		local s = p.h
		local y = p.y - camera_y
		local left_x = p.x - p.w / 2 + s / 2
		local right_x = p.x + p.w / 2 - s / 2
		local mid_w = math.max(0, p.w - 2 * s)
		-- left cap
		Renderer:queue_command(
			RenderCommand
				:new()
				:with(PLATFORM_CAP)
				:with_position(left_x, y)
				:with_scale(s, s)
		)
		-- middle stretch
		if mid_w > 0 then
			Renderer:queue_command(
				RenderCommand
					:new()
					:with(PLATFORM_MID)
					:with_position(p.x, y)
					:with_scale(mid_w, s)
			)
		end
		-- right cap
		Renderer:queue_command(
			RenderCommand
				:new()
				:with(PLATFORM_CAP)
				:with_position(right_x, y)
				:with_scale(-s, s)
		)
	end

	Renderer:queue_command(
		RenderCommand
			:new()
			:with(PLAYER)
            :with(Color.RED)
			:with_position(player.x, player.y - camera_y)
			:with_scale(PLAYER_WIDTH, PLAYER_HEIGHT)
	)
end


