class Pong: PixelGameEngine
{
	// static constructor style
	// cause i really want to type Doom.Run(); lol
	private static ref Pong m_Instance;
	
	static void Run()
	{
		/*if (GetGame().IsServer()) {
			return;
		}*/
		
		if (m_Instance) {
			delete m_Instance;
		}
		
		m_Instance = new Pong();	
	}
	
	static void Stop()
	{
		delete m_Instance;
	}
	
	static Pong GetInstance()
	{
		return m_Instance;
	}
	
	const int COLOR_WHITE = ARGB(255, 255, 255, 255);
	const int PADDLE_SIZE = 8;
	const float PADDLE_SPEED = 0.05;
	
	protected int player_1_pos = 0;
	protected int player_2_pos = 0;
	
	override void OnFrame()
	{
		super.OnFrame();
		
		Input input = GetGame().GetInput();
		
		if (input.LocalValue("UAMoveForward")) {
			SetPlayerPosition(player_1_pos + 1);
		}
		
		if (input.LocalValue("UAMoveBack")) {
			SetPlayerPosition(player_1_pos - 1);
		}
		
		
		// player 1
		DrawLine(0, player_1_pos + (PADDLE_SIZE / 2), 0, player_1_pos - (PADDLE_SIZE / 2), COLOR_WHITE);
		
		// player 2
		DrawLine(SCREEN_WIDTH - 1, player_2_pos + (PADDLE_SIZE / 2), SCREEN_WIDTH - 1, player_2_pos - (PADDLE_SIZE / 2), COLOR_WHITE);
	}
	
	void SetPlayer1Position(int position)
	{
		if (player_1_pos - (PADDLE_SIZE / 2) + position <= 0) {
			 position = (PADDLE_SIZE / 2);
		}
		
		if (player_1_pos + (PADDLE_SIZE / 2) > SCREEN_HEIGHT) {
			position = SCREEN_HEIGHT - (PADDLE_SIZE / 2);
		}
		
		player_1_pos = position;
	}
}