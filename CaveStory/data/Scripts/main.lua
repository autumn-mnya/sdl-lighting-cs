local lighttest = ModCS.Rect.Create(0, 0, 256, 256)

function ModCS.Game.Draw()
	--ModCS.Rect.Put(lighttest, 0, 0, 41)
	
	ModCS.PutNumber(ModCS.Lighting.GetLight(), 0, 0)
end

function ModCS.Game.Act()
	if ModCS.Key.Arms(true) then
		ModCS.Lighting.SetLight(ModCS.Lighting.GetLight() + 1)
	end
	
	if ModCS.Key.ArmsRev(true) then
		ModCS.Lighting.SetLight(ModCS.Lighting.GetLight() - 1)
	end
	
	if ModCS.Lighting.GetLight() > 255 then
		ModCS.Lighting.SetLight(0)
	end
	
	if ModCS.Key.Map() then
		if ModCS.Lighting.GetStageLighting() == false then
			ModCS.Lighting.SetStageLighting(true)
		else
			ModCS.Lighting.SetStageLighting(false)
		end
	end
	
end

-- Set Light Level (SLL)
function ModCS.Tsc.Command.SLL()
	local level = ModCS.Tsc.GetArgument(1)
	ModCS.Lighting.SetLight(level)
end