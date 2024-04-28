Hud = {
	seconds_elapsed = 0,
	finish = false,

	OnStart = function(self)
    Event.Subscribe("event_victory", self, self.OnEventVictory)
	end,

	OnUpdate = function(self)
    local curr_delta_time = Time.DeltaTime();
    
		if curr_delta_time > 0 and self.finish == false then
      self.seconds_elapsed = self.seconds_elapsed + curr_delta_time
		end

		local text = string.format("Time : %.1f", self.seconds_elapsed)
		local x = 10
		local y = 10

		if self.finish then
      text = string.format("FINISH! Final Time : %.3f seconds!", self.seconds_elapsed)
		end
    

		Text.Draw(text, x, y, "NotoSans-Regular", 24, 0, 0, 0, 255)
	end,

	OnEventVictory = function(self)
    self.finish = true
	end
}

