MovingBox = {
	OnStart = function(self)
		self.rb = self.actor:GetComponent("Rigidbody")
		local end_pos = Vector2(self.rb.x, self.rb.y + 3)
		-- GOTween.To(self, function(self) return self.rb:GetPosition() end, function(self, new_pos) self.rb:MovePosition(new_pos) end, end_pos, 2.0):SetLoops(-1, LoopType.Yoyo):SetEase(EaseType.Linear):SetUpdate(UpdateType.Fixed)
		self.rb:GOMove(end_pos, 2):SetLoops(-1, LoopType.Yoyo):SetEase(EaseType.Linear):SetUpdate(UpdateType.Fixed)
		GOTween.To(self, function(self) return self.rb:GetRotation() end, function(self, new_pos) self.rb:SetRotation(new_pos) end, 360, 1.0):SetLoops(-1, LoopType.Incremental):SetEase(EaseType.OutQuad):SetUpdate(UpdateType.Fixed)
	end

}