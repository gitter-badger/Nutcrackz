--This simple script creates a blank scene and adds a gameObject to it.
--You can try and mess around with it to suit your needs :)

local gos = gameObject()
	--gos:setFilepath("C:\\Users\\David\\Desktop\\Diverse\\ArmyPilot\\ArmyPilot.dae")
	--gos:setFilepath("C:\\Users\\David\\Desktop\\Diverse\\sinon-sword-art-online.fbx")
	gos:setFilepath(".\\Models\\nanosuit.obj")
	gos:setId(1)
	gos:setName("ArmyPilot")
	gos:setPosition(5, 0, 0)
	gos:setRotation(0, -45, 0)
	gos:setScale(1, 1, 1)
	gos:setGameObject()

AddGameObject(gos)

local gos2 = gameObject()
	--gos2:setFilepath("C:\\Users\\David\\Desktop\\Diverse\\ArmyPilot\\ArmyPilot.dae")
	gos2:setFilepath("C:\\Users\\David\\Desktop\\samples\\duck_triangulate.dae")
	--gos:setFilepath(".\\Models\\nanosuit.obj")
	--gos2:setFilepath("C:\\Users\\David\\Desktop\\samples\\duck.dae")
	gos2:setId(2)
	gos2:setName("ArmyPilot")
	gos2:setPosition(-5, 0, 0)
	gos2:setRotation(0, 45, 0)
	gos2:setScale(0.1, 0.1, 0.1)
	gos2:setGameObject()

AddGameObject(gos2)