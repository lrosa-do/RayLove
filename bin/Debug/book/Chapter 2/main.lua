
sets = Settings.new()

--define scenes
sceneManager = SceneManager.new({
	--start scene
	["start"] = startScene,
	--about scene
	["about"] = aboutScene,
	--options scene
	["options"] = optionsScene,
})
--add manager to stage
stage:addChild(sceneManager)

--go to start scene
sceneManager:changeScene("start", conf.transitionTime, conf.transition, conf.easing)
