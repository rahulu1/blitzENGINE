GameManager = {

	-- TILE CODES --
	-- 0 : nothing
	-- 1 : Static box
	-- 2 : player

	-- stage1 = {
	-- 	{1, 0, 0, 5, 0, 4, 4, 4, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, -- 20x20
	-- 	{1, 2, 0, 0, 0, 4, 4, 4, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	-- 	{1, 1, 0, 0, 0, 4, 4, 4, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	-- 	{1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	-- 	{1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1},
	-- 	{1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1},
	-- 	{1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1},
	-- 	{1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1},
	-- 	{1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1},
	-- 	{1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1},
	-- 	{1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1},
	-- 	{1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1},
	-- 	{1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1},
	-- 	{1, 1, 0, 1, 1, 1, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1},
	-- 	{1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 3, 3, 1, 1, 1, 0, 0, 1},
	-- 	{1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1},
	-- 	{1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1},
	-- 	{1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1},
	-- 	{1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1},
	-- 	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1},
	-- },

	stage1 = {
		{1, 0, 0, 2, 0, 0, 3, 0, 0, 4, 0, 0, 5, 0, 0, 6, 0, 0, 7, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{8, 0, 0, 9, 0, 0, 10, 0, 0, 11, 0, 0, 12, 0, 0, 13, 0, 0, 14, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{15, 0, 0, 16, 0, 0, 17, 0, 0, 18, 0, 0, 19, 0, 0, 20, 0, 0, 21, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{22, 0, 0, 23, 0, 0, 24, 0, 0, 25, 0, 0, 26, 0, 0, 27, 0, 0, 28, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{29, 0, 0, 30, 0, 0, 31, 0, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	},

	OnStart = function(self)
		-- Spawn stage
		for y=1,20 do 
			for x = 1,20 do
				local tile_code = self.stage1[y][x]
				local tile_pos = Vector2(x, y)

				-- if tile_code == 2 then
				-- 	local new_player = Actor.Instantiate("Player")
				-- 	local new_player_rb = new_player:GetComponent("Rigidbody")
				-- 	new_player_rb.x = tile_pos.x
				-- 	new_player_rb.y = tile_pos.y
				
				-- elseif tile_code == 1 then
				-- 	local new_box = Actor.Instantiate("KinematicBox")
				-- 	local new_box_rb = new_box:GetComponent("Rigidbody")
				-- 	new_box_rb.x = tile_pos.x
				-- 	new_box_rb.y = tile_pos.y

				-- elseif tile_code == 3 then
				-- 	local new_box = Actor.Instantiate("BouncyBox")
				-- 	local new_box_rb = new_box:GetComponent("Rigidbody")
				-- 	new_box_rb.x = tile_pos.x
				-- 	new_box_rb.y = tile_pos.y
				
				-- elseif tile_code == 4 then
				-- 	local new_box = Actor.Instantiate("VictoryBox")
				-- 	local new_box_rb = new_box:GetComponent("Rigidbody")
				-- 	new_box_rb.x = tile_pos.x
				-- 	new_box_rb.y = tile_pos.y
				
				-- elseif tile_code == 5 then
				-- 	local new_box = Actor.Instantiate("KinematicBox")
				-- 	new_box:AddComponent("MovingBox")
				-- 	local new_box_rb = new_box:GetComponent("Rigidbody")
				-- 	new_box_rb.x = tile_pos.x
				-- 	new_box_rb.y = tile_pos.y
				-- end

				if tile_code > 0 then
					local new_box = Actor.Instantiate("KinematicBox")
					local new_box_rb = new_box:GetComponent("Rigidbody")
					new_box_rb.x = tile_pos.x
					new_box_rb.y = tile_pos.y
					

					if tile_code == 1 then
						self.linear_rb = new_box_rb
						GOTween.To(self, function(self) return self.linear_rb:GetPosition() end, function(self, new_pos) self.linear_rb:MovePosition(new_pos) end, Vector2(tile_pos.x + 1, tile_pos.y), 1):SetLoops(-1, LoopType.Yoyo):SetEase(EaseType.Linear)

					elseif tile_code == 2 then
						self.inquad_rb = new_box_rb
						GOTween.To(self, function(self) return self.inquad_rb:GetPosition() end, function(self, new_pos) self.inquad_rb:MovePosition(new_pos) end, Vector2(tile_pos.x + 1, tile_pos.y), 1):SetLoops(-1, LoopType.Yoyo):SetEase(EaseType.InQuad)
					
					elseif tile_code == 3 then
						self.outquad_rb = new_box_rb
						GOTween.To(self, function(self) return self.outquad_rb:GetPosition() end, function(self, new_pos) self.outquad_rb:MovePosition(new_pos) end, Vector2(tile_pos.x + 1, tile_pos.y), 1):SetLoops(-1, LoopType.Yoyo):SetEase(EaseType.OutQuad)

					elseif tile_code == 4 then
						self.inoutquad_rb = new_box_rb
						GOTween.To(self, function(self) return self.inoutquad_rb:GetPosition() end, function(self, new_pos) self.inoutquad_rb:MovePosition(new_pos) end, Vector2(tile_pos.x + 1, tile_pos.y), 1):SetLoops(-1, LoopType.Yoyo):SetEase(EaseType.InOutQuad)
					
					elseif tile_code == 5 then
						self.inquart_rb = new_box_rb
						GOTween.To(self, function(self) return self.inquart_rb:GetPosition() end, function(self, new_pos) self.inquart_rb:MovePosition(new_pos) end, Vector2(tile_pos.x + 1, tile_pos.y), 1):SetLoops(-1, LoopType.Yoyo):SetEase(EaseType.InQuart)
					
					elseif tile_code == 6 then
						self.outquart_rb = new_box_rb
						GOTween.To(self, function(self) return self.outquart_rb:GetPosition() end, function(self, new_pos) self.outquart_rb:MovePosition(new_pos) end, Vector2(tile_pos.x + 1, tile_pos.y), 1):SetLoops(-1, LoopType.Yoyo):SetEase(EaseType.OutQuart)

					elseif tile_code == 7 then
						self.inoutquart_rb = new_box_rb
						GOTween.To(self, function(self) return self.inoutquart_rb:GetPosition() end, function(self, new_pos) self.inoutquart_rb:MovePosition(new_pos) end, Vector2(tile_pos.x + 1, tile_pos.y), 1):SetLoops(-1, LoopType.Yoyo):SetEase(EaseType.InOutQuart)
					
					elseif tile_code == 8 then
						self.inquint_rb = new_box_rb
						GOTween.To(self, function(self) return self.inquint_rb:GetPosition() end, function(self, new_pos) self.inquint_rb:MovePosition(new_pos) end, Vector2(tile_pos.x + 1, tile_pos.y), 1):SetLoops(-1, LoopType.Yoyo):SetEase(EaseType.InQuint)

					elseif tile_code == 9 then
						self.outquint_rb = new_box_rb
						GOTween.To(self, function(self) return self.outquint_rb:GetPosition() end, function(self, new_pos) self.outquint_rb:MovePosition(new_pos) end, Vector2(tile_pos.x + 1, tile_pos.y), 1):SetLoops(-1, LoopType.Yoyo):SetEase(EaseType.OutQuint)
					
					elseif tile_code == 10 then
						self.inoutquint_rb = new_box_rb
						GOTween.To(self, function(self) return self.inoutquint_rb:GetPosition() end, function(self, new_pos) self.inoutquint_rb:MovePosition(new_pos) end, Vector2(tile_pos.x + 1, tile_pos.y), 1):SetLoops(-1, LoopType.Yoyo):SetEase(EaseType.InOutQuint)
					
					elseif tile_code == 11 then
						self.inexpo_rb = new_box_rb
						GOTween.To(self, function(self) return self.inexpo_rb:GetPosition() end, function(self, new_pos) self.inexpo_rb:MovePosition(new_pos) end, Vector2(tile_pos.x + 1, tile_pos.y), 1):SetLoops(-1, LoopType.Yoyo):SetEase(EaseType.InExpo)

					elseif tile_code == 12 then
						self.outexpo_rb = new_box_rb
						GOTween.To(self, function(self) return self.outexpo_rb:GetPosition() end, function(self, new_pos) self.outexpo_rb:MovePosition(new_pos) end, Vector2(tile_pos.x + 1, tile_pos.y), 1):SetLoops(-1, LoopType.Yoyo):SetEase(EaseType.OutExpo)

					elseif tile_code == 13 then
						self.inoutexpo_rb = new_box_rb
						GOTween.To(self, function(self) return self.inoutexpo_rb:GetPosition() end, function(self, new_pos) self.inoutexpo_rb:MovePosition(new_pos) end, Vector2(tile_pos.x + 1, tile_pos.y), 1):SetLoops(-1, LoopType.Yoyo):SetEase(EaseType.InOutExpo)

					elseif tile_code == 14 then
						self.incirc_rb = new_box_rb
						GOTween.To(self, function(self) return self.incirc_rb:GetPosition() end, function(self, new_pos) self.incirc_rb:MovePosition(new_pos) end, Vector2(tile_pos.x + 1, tile_pos.y), 1):SetLoops(-1, LoopType.Yoyo):SetEase(EaseType.InCirc)

					elseif tile_code == 15 then
						self.outcirc_rb = new_box_rb
						GOTween.To(self, function(self) return self.outcirc_rb:GetPosition() end, function(self, new_pos) self.outcirc_rb:MovePosition(new_pos) end, Vector2(tile_pos.x + 1, tile_pos.y), 1):SetLoops(-1, LoopType.Yoyo):SetEase(EaseType.OutCirc)

					elseif tile_code == 16 then
						self.inoutcirc_rb = new_box_rb
						GOTween.To(self, function(self) return self.inoutcirc_rb:GetPosition() end, function(self, new_pos) self.inoutcirc_rb:MovePosition(new_pos) end, Vector2(tile_pos.x + 1, tile_pos.y), 1):SetLoops(-1, LoopType.Yoyo):SetEase(EaseType.InOutCirc)

					elseif tile_code == 17 then
						self.inback_rb = new_box_rb
						GOTween.To(self, function(self) return self.inback_rb:GetPosition() end, function(self, new_pos) self.inback_rb:MovePosition(new_pos) end, Vector2(tile_pos.x + 1, tile_pos.y), 1):SetLoops(-1, LoopType.Yoyo):SetEase(EaseType.InBack)

					elseif tile_code == 18 then
						self.outback_rb = new_box_rb
						GOTween.To(self, function(self) return self.outback_rb:GetPosition() end, function(self, new_pos) self.outback_rb:MovePosition(new_pos) end, Vector2(tile_pos.x + 1, tile_pos.y), 1):SetLoops(-1, LoopType.Yoyo):SetEase(EaseType.OutBack)

					elseif tile_code == 19 then
						self.inoutback_rb = new_box_rb
						GOTween.To(self, function(self) return self.inoutback_rb:GetPosition() end, function(self, new_pos) self.inoutback_rb:MovePosition(new_pos) end, Vector2(tile_pos.x + 1, tile_pos.y), 1):SetLoops(-1, LoopType.Yoyo):SetEase(EaseType.InOutBack)

					elseif tile_code == 20 then
						self.inelastic_rb = new_box_rb
						GOTween.To(self, function(self) return self.inelastic_rb:GetPosition() end, function(self, new_pos) self.inelastic_rb:MovePosition(new_pos) end, Vector2(tile_pos.x + 1, tile_pos.y), 1):SetLoops(-1, LoopType.Yoyo):SetEase(EaseType.InElastic)

					elseif tile_code == 21 then
						self.outelastic_rb = new_box_rb
						GOTween.To(self, function(self) return self.outelastic_rb:GetPosition() end, function(self, new_pos) self.outelastic_rb:MovePosition(new_pos) end, Vector2(tile_pos.x + 1, tile_pos.y), 1):SetLoops(-1, LoopType.Yoyo):SetEase(EaseType.OutElastic)

					elseif tile_code == 22 then
						self.inoutelastic_rb = new_box_rb
						GOTween.To(self, function(self) return self.inoutelastic_rb:GetPosition() end, function(self, new_pos) self.inoutelastic_rb:MovePosition(new_pos) end, Vector2(tile_pos.x + 1, tile_pos.y), 1):SetLoops(-1, LoopType.Yoyo):SetEase(EaseType.InOutElastic)

					elseif tile_code == 23 then
						self.inbounce_rb = new_box_rb
						GOTween.To(self, function(self) return self.inbounce_rb:GetPosition() end, function(self, new_pos) self.inbounce_rb:MovePosition(new_pos) end, Vector2(tile_pos.x + 1, tile_pos.y), 1):SetLoops(-1, LoopType.Yoyo):SetEase(EaseType.InBounce)

					elseif tile_code == 24 then
						self.outbounce_rb = new_box_rb
						GOTween.To(self, function(self) return self.outbounce_rb:GetPosition() end, function(self, new_pos) self.outbounce_rb:MovePosition(new_pos) end, Vector2(tile_pos.x + 1, tile_pos.y), 1):SetLoops(-1, LoopType.Yoyo):SetEase(EaseType.OutBounce)

					elseif tile_code == 25 then
						self.inoutbounce_rb = new_box_rb
						GOTween.To(self, function(self) return self.inoutbounce_rb:GetPosition() end, function(self, new_pos) self.inoutbounce_rb:MovePosition(new_pos) end, Vector2(tile_pos.x + 1, tile_pos.y), 1):SetLoops(-1, LoopType.Yoyo):SetEase(EaseType.InOutBounce)

					elseif tile_code == 26 then
						self.incubic_rb = new_box_rb
						GOTween.To(self, function(self) return self.incubic_rb:GetPosition() end, function(self, new_pos) self.incubic_rb:MovePosition(new_pos) end, Vector2(tile_pos.x + 1, tile_pos.y), 1):SetLoops(-1, LoopType.Yoyo):SetEase(EaseType.InCubic)
						
					elseif tile_code == 27 then
						self.outcubic_rb = new_box_rb
						GOTween.To(self, function(self) return self.outcubic_rb:GetPosition() end, function(self, new_pos) self.outcubic_rb:MovePosition(new_pos) end, Vector2(tile_pos.x + 1, tile_pos.y), 1):SetLoops(-1, LoopType.Yoyo):SetEase(EaseType.OutCubic)

					elseif tile_code == 28 then
						self.inoutcubic_rb = new_box_rb
						GOTween.To(self, function(self) return self.inoutcubic_rb:GetPosition() end, function(self, new_pos) self.inoutcubic_rb:MovePosition(new_pos) end, Vector2(tile_pos.x + 1, tile_pos.y), 1):SetLoops(-1, LoopType.Yoyo):SetEase(EaseType.InOutCubic)

					elseif tile_code == 29 then
						self.insine_rb = new_box_rb
						GOTween.To(self, function(self) return self.insine_rb:GetPosition() end, function(self, new_pos) self.insine_rb:MovePosition(new_pos) end, Vector2(tile_pos.x + 1, tile_pos.y), 1):SetLoops(-1, LoopType.Yoyo):SetEase(EaseType.InSine)

					elseif tile_code == 30 then
						self.outsine_rb = new_box_rb
						GOTween.To(self, function(self) return self.outsine_rb:GetPosition() end, function(self, new_pos) self.outsine_rb:MovePosition(new_pos) end, Vector2(tile_pos.x + 1, tile_pos.y), 1):SetLoops(-1, LoopType.Yoyo):SetEase(EaseType.OutSine)

					elseif tile_code == 31 then
						self.inoutsine_rb = new_box_rb
						self.inoutsine_rb:GOMove(Vector2(tile_pos.x + 1, tile_pos.y), 1):SetLoops(-1, LoopType.Yoyo):SetEase(EaseType.InOutSine)
						-- GOTween.To(self, function(self) return self.inoutsine_rb:GetPosition() end, function(self, new_pos) self.inoutsine_rb:MovePosition(new_pos) end, Vector2(tile_pos.x + 1, tile_pos.y), 1):SetLoops(-1, LoopType.Yoyo):SetEase(EaseType.InOutSine)
					end

				elseif tile_code < 0 then
					local new_box = Actor.Instantiate("AnimatedBox")
					local new_box_rb = new_box:GetComponent("Rigidbody")
					new_box_rb.x = tile_pos.x
					new_box_rb.y = tile_pos.y
				end
			end
		end
	end,

	OnUpdate = function(self)
		
	end
}

