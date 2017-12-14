using TheEngine;
using TheEngine.TheConsole;

	public class Nngf 
	{
		//public TheGameObject go;
		// Use this for initialization
		//TheGameObject go = new TheGameObject();
		void Start ()
		{
			TheGameObject go2 = new TheGameObject();
			//GameObject go = null;
			go2.name = "Hi!";
			TheConsole.Log(go2.GetComponent<TheTransform>().LocalPosition);
			TheConsole.Log(go2.GetComponent<TheTransform>().GlobalPosition);
			TheGameObject go5 = TheGameObject.Duplicate(go2);
			go5.SetParent(go2);
			TheGameObject go8 = go2.GetChild(0);
			go8.name = "bla";
			//TheConsole.Log(go2.AddComponent<TheTransform>().Position = new TheVector3( 30,0,0));
			//TheConsole.Log(go2.GetComponent<TheTransform>().Position);
			//GameObject.Self.SetName("di8i09u00o0iopouid");
			//GameObject go3 = new GameObject ();
			//GameObject.Self.SetName("di8i09u00o0iopouid");
			//go3.SetName("Hi!");
			//go3.SetActive(false);
			if(!TheGameObject.Self.IsActive())
			{
				TheGameObject.Self.SetActive(false);
			}
			if(go2.IsActive())
			{
				go2.SetActive(false);
			}
			TheConsole.Log(go2.name);
			go2 = TheGameObject.Self;
			go2.name = "New";
			//go2.AddComponent<TheTransform>().Position = new TheVector3( 30,10,0);
			//TheConsole.Log(go2.GetComponent<TheTransform>().Position);
			//TheConsole.Log(TheGameObject.Self.GetComponent<TheTransform>().Position);
			
		}
		
		// Update is called once per frame
		void Update ()
		{
			if(TheInput.IsMouseButtonDown(1))
			{
				TheConsole.Log("Button 0");
			}
			if(TheInput.IsKeyDown("a"))
			{
				TheConsole.Log("key a");
			}
		}
	}
	