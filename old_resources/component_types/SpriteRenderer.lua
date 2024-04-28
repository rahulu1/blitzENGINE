SpriteRenderer = {
	sprite = "???",
	r = 255,
	g = 255,
	b = 255,
	a = 255,
	sorting_order = 0,
	game_over = false,

	OnStart = function(self)
		self.pos = Vector2(0, 0)
		self.rot_degrees = 0
		self.color_tween = GOTween.To(self, function(self) return self.r end, function(self, new_color) self.r = new_color end, 0, 4.0):SetLoops(-1, LoopType.Yoyo)
		-- Event.Subscribe("event_victory", self, self.OnEventVictory)
	end,

	OnUpdate = function(self)
		self.rb = self.actor:GetComponent("Rigidbody")

		if self.rb ~= nil then
			self.pos = self.rb:GetPosition()
			self.rot_degrees = self.rb:GetRotation()
		end

		Image.DrawEx(self.sprite, self.pos.x, self.pos.y, self.rot_degrees, 1.0, 1.0, 0.5, 0.5, self.r, self.g, self.b, self.a, self.sorting_order)
	end,

	OnEventVictory = function(self)
		if self.game_over == false then
			self.game_over = true
			self.color_tween:Kill()
			self.r = 0
			GOTween.To(self, function(self) return self.r end, function(self, new_color) self.r = new_color end, 160, 3.0):SetEase(EaseType.OutElastic)
		end
	end
  
}

