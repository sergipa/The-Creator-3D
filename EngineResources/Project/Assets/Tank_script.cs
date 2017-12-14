using TheEngine;

public class Tank_script {

	public TheGameObject tank;
	
	void Start () {

	}
	
	void Update () {
		if(TheInput.IsKeyRepeat("UP_ARROW"))
        {
            tank.GetComponent<TheTransform>().GlobalPosition += TheVector3.Forward;
        }
        else if (TheInput.IsKeyRepeat("DOWN_ARROW"))
        {
            tank.GetComponent<TheTransform>().GlobalPosition += TheVector3.BackWard;
        }
        else if (TheInput.IsKeyRepeat("LEFT_ARROW"))
        {
            tank.GetComponent<TheTransform>().GlobalRotation += TheVector3.BackWard;
        }
        else if (TheInput.IsKeyRepeat("RIGHT_ARROW"))
        {
            tank.GetComponent<TheTransform>().GlobalRotation += TheVector3.BackWard;
        }
	}
}