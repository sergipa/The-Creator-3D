using TheEngine;

public class Tank_script {

	public TheGameObject tank;
	
	void Start () {
		tank.SetActive(false);
	}
	
	void Update () {
		if(TheInput.IsKeyDown("UP_ARROW"))
        {
            tank.GetComponent<TheTransform>().GlobalPosition += TheVector3.Forward;
        }
        else if (TheInput.IsKeyDown("DOWN_ARROW"))
        {
            tank.GetComponent<TheTransform>().GlobalPosition += TheVector3.BackWard;
        }
        else if (TheInput.IsKeyDown("LEFT_ARROW"))
        {
            tank.GetComponent<TheTransform>().GlobalRotation += TheVector3.BackWard;
        }
        else if (TheInput.IsKeyDown("RIGHT_ARROW"))
        {
            tank.GetComponent<TheTransform>().GlobalRotation += TheVector3.BackWard;
        }
	}
}