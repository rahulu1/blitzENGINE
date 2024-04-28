BouncyBox = {

	is_bouncing = false,

	OnStart = function(self)
		self.rb = self.actor:GetComponent("Rigidbody")
	end,

	OnCollisionEnter = function(self, collision)
    
    self.sr = self.actor:GetComponent("SpriteRenderer")
--    Tween.Demo(self.sr, "r", 0, 1.0, 1, false, true)
--    Tween.Demo(self.sr, "g", 0, 1.0, 1, false, true)
    
		if collision.other:GetName() == "player" then
			if self.is_bouncing == false then
				self.is_bouncing = true
				GOTween.To(self, function(self) return self.rb:GetPosition() end, function(self, new_pos) self.rb:MovePosition(new_pos) end, Vector2(self.rb:GetPosition().x, self.rb:GetPosition().y + 0.3), 0.5):SetLoops(2, LoopType.Yoyo):SetEase(EaseType.OutElastic):OnKill(self.OnBounceKill, self):SetUpdate(UpdateType.Fixed)
			end
		end
	end,

	OnBounceKill = function(self)
		self.rb:SetVelocity(Vector2(0, 0))
		self.is_bouncing = false
	end
}

