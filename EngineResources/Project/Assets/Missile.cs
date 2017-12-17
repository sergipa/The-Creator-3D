using TheEngine;

public class Missile {

	int speed = 30;
	
	void Start () {
		
	}
	
	void Update () {
		if(TheGameObject.Self.IsActive())
        {
            TheGameObject.Self.GetComponent<TheTransform>().GlobalPosition += TheGameObject.Self.GetComponent<TheTransform>().ForwardDirection * Time.DeltaTime * speed;
        }
	}
}