KeyboardControls = {
  speed = 1000.0,
  jump_power = 1000.0,

	OnStart = function(self)
    self.rb = self.actor:GetComponent("Rigidbody")
    self.sr = self.actor:GetComponent("SpriteRenderer")
    
--    Tween.Demo(self.sr, "a", 64, 1.0, 4, false, true)
    
    self.jump_power = self.jump_power * self.rb:GetMass()
	end,

	OnUpdate = function(self)

		local curr_delta_time = Time.DeltaTime();

		-- Horizontal
		local horizontal_input = 0
		if Input.GetKey("right") then
			horizontal_input = self.speed * curr_delta_time
		end

		if Input.GetKey("left") then
			horizontal_input = -self.speed * curr_delta_time
		end

		-- Vertical
		local vertical_input = 0

		-- Check on ground
		local on_ground = false
		ground_object = Physics.Raycast(self.rb:GetPosition(), Vector2(0, 1), 0.5)
		if Input.GetKeyDown("up") or Input.GetKeyDown("space") then
			if ground_object ~= nil then
				vertical_input = -self.jump_power
			end
		end

		self.rb:AddForce(Vector2(horizontal_input, vertical_input))
  end
}

