using TheEngine;
using TheEngine.TheConsole;

public class Tank_Shoot {

    TheFactory missile_factory;
    public TheGameObject missile_spawner;

	void Start () {
	    missile_factory = TheGameObject.Self.GetComponent<TheFactory>();
        missile_factory.StartFactory();
	}
	
	void Update () {
        if(TheInput.IsMouseButtonUp(1))
        {
            missile_factory.SetSpawnPosition(missile_spawner.GetComponent<TheTransform>().GlobalPosition);
            missile_factory.SetSpawnRotation(missile_spawner.GetComponent<TheTransform>().GlobalRotation);
            missile_factory.Spawn();
        }
	}
}