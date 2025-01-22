function CalcRandomPos()
	return Vector2i:new(math.random(0, world_size.x - 1), math.random(0, world_size.y - 1))
end

function NewGame()
	snake = { { color=Colour.DarkGreen, pos=CalcRandomPos() } }
	apple = CalcRandomPos()

	snake_dir = Vector2i:new(1, 0)

	counter = 0.0
	delay = 0.15

	score = 0
	is_dead = false
end

function OnCreate()
	math.randomseed(os.time())

	tile_size = Vector2i:new(8, 8)
	world_size = app:GetScreenSize() // tile_size
	NewGame()
	
	return true
end

function RandomPixel()
	return Pixel:new(math.random(0, 255), math.random(0, 255), math.random(0, 255), 255)
end

function OnUpdate(dt)
	if is_dead then
		if app:GetKey(Key.Space).pressed then NewGame() end

		app:DrawString(1, app:ScreenHeight() // 2, "Game over! Press SPACE to play again.", Colour.Yellow, 1, 1)

		return true
	end

	if app:GetKey(Key.Left).pressed then snake_dir = Vector2i:new(-1, 0) end
	if app:GetKey(Key.Right).pressed then snake_dir = Vector2i:new(1, 0) end
	if app:GetKey(Key.Up).pressed then snake_dir = Vector2i:new(0, -1) end
	if app:GetKey(Key.Down).pressed then snake_dir = Vector2i:new(0, 1) end

	if counter > delay then
		counter = 0.0

		table.insert(snake, 1, { color = RandomPixel(), pos = snake[1].pos + snake_dir })

		if snake[1].pos == apple then
			score = score + 1

			if score % 5 == 0 then
				delay = delay - 0.001
			end

			apple = CalcRandomPos()
			snake[#snake + 1] = snake[#snake]
		end

		for i=2, #snake do
			snake[i].color = RandomPixel()

			if snake[i].pos == snake[1].pos then
				is_dead = true
			end
		end

		if snake[1].pos.x < 0 or snake[1].pos.y < 0 or snake[1].pos.x >= world_size.x or snake[1].pos.y >= world_size.y then
			is_dead = true
		end

		table.remove(snake, #snake)
	end

	counter = counter + dt

	app:Clear(Pixel:new(32, 32, 32, 255))

	app:DrawString(2, 2, "Score: " .. score, Colour.Yellow, 1, 1)

	for k, v in pairs(snake) do
		app:FillRectangle(tile_size * v.pos, tile_size, v.color)
	end

	app:FillRectangle(tile_size * apple, tile_size, Colour.Red)

	return true
end

function CreateApp()
	return
	{
		size = { 256, 240, 4, 4 },
		title = "Snake"
	}
end