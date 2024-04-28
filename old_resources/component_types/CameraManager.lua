CameraManager = {
	ease_factor = 12,
	tracking_player = false,
	camera_shake = Vector2(-0.25, -0.3),
	is_shaking = false,

	OnStart = function(self)
		Camera.SetZoom(0.3)
	end,

	OnLateUpdate = function(self)
		local player_actor = Actor.Find("player")
		if player_actor == nil then
			self.tracking_player = false
			Camera.SetPosition(10, 7)
			return
		
		elseif self.tracking_player == false then
			self.tracking_player = true
			local rb = player_actor:GetComponent("Rigidbody")
			local player_pos = rb:GetPosition()
			Camera.SetPosition(player_pos.x, player_pos.y)
			return
		end

		Camera.SetZoom(0.75)

		local shake_apply = Vector2(0, 0)
		if self.is_shaking == false and Input.GetKey("q") then
			GOTween.To(self, function(self) return self.camera_shake.x end, function(self, new_x) self.camera_shake.x = new_x end, 0.25, 0.1):SetLoops(10, LoopType.Yoyo):OnKill(function(self) self.is_shaking = false end)
			GOTween.To(self, function(self) return self.camera_shake.y end, function(self, new_y) self.camera_shake.y = new_y end, 0.3, 0.25):SetLoops(4, LoopType.Yoyo):OnKill(function(self) self.is_shaking = false end)
			-- Tween.Demo(self, function(self) return self.camera_shake.x end, function(self, new_x) self.camera_shake.x = new_x end, 0.25, 0.1, 10, true, false, function(self) self.is_shaking = false end)
			-- Tween.Demo(self, function(self) return self.camera_shake.y end, function(self, new_y) self.camera_shake.y = new_y end, 0.3, 0.25, 4, true, false, function(self) self.is_shaking = false end)
			self.is_shaking = true
		end

		if is_shaking then
			shake_apply = self.camera_shake
		end

		local player_rb = player_actor:GetComponent("Rigidbody")
		local desired_position = player_rb:GetPosition()
		local current_position = Vector2(Camera.GetPositionX(), Camera.GetPositionY())

		local new_position = current_position + (desired_position - current_position + self.camera_shake) * self.ease_factor * Time.DeltaTime()
		Camera.SetPosition(new_position.x, new_position.y)
	end
  
}

