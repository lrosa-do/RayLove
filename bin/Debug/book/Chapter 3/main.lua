require "box2d"
sets = Settings.new()
gm = GameManager.new()

--define scenes
sceneManager = SceneManager.new({
	--start scene
	["start"] = startScene,
	--about scene
	["about"] = aboutScene,
	--options scene
	["options"] = optionsScene,
	--level scene
	["level"] = levelScene,
	--level select scene
	["levelselect"] = levelselectScene,
})
--add manager to stage
stage:addChild(sceneManager)

--go to start scene
sceneManager:changeScene("start", conf.transitionTime, conf.transition, conf.easing)
