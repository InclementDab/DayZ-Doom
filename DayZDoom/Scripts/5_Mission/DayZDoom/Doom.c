class Doom
{
	// static constructor style
	// cause i really want to type Doom.Run(); lol
	private static ref Doom m_Instance;
	
	static void Run()
	{
		/*if (GetGame().IsServer()) {
			return;
		}*/
		
		if (m_Instance) {
			delete m_Instance;
		}
		
		m_Instance = new Doom();	
	}
	
	static void Stop()
	{
		m_Instance.Stop();
	}
	
	static Doom GetInstance()
	{
		return m_Instance;
	}
	
	static const int SCREEN_WIDTH = 20;
	static const int SCREEN_HEIGHT = 10;
	
	static const int PIXEL_SIZE = 1;
	
	protected Widget m_LayoutRoot;
	protected ref array<ref array<Widget>> m_ScreenPixels = {};
	
	private void Doom()
	{
		m_LayoutRoot = GetGame().GetWorkspace().CreateWidgets("DayZDoom/GUI/layouts/panel_display.layout");
		for (int i = 0; i < SCREEN_WIDTH; i++) {
			if (!m_ScreenPixels[i]) {
				m_ScreenPixels[i] = {};
			}
			
			Print(m_ScreenPixels[i]);
			for (int j = 0; j < SCREEN_HEIGHT; j++) {
				m_ScreenPixels[i][j] = GetGame().GetWorkspace().CreateWidget(FrameWidgetTypeID, i * PIXEL_SIZE, j * PIXEL_SIZE, PIXEL_SIZE, PIXEL_SIZE, WidgetFlags.VISIBLE | WidgetFlags.IGNOREPOINTER, COLOR_YELLOW, 0, m_LayoutRoot);
				Print(m_ScreenPixels[i][j]);
				//SetPixelColor(i, j, COLOR_WHITE);
			}
		}
	}
	
	void ~Doom()
	{
		delete m_ScreenPixels;
		if (m_LayoutRoot) {
			m_LayoutRoot.Unlink();
			delete m_LayoutRoot;	
		}
	}
	
	void SetPixelColor(int pixel_x, int pixel_y, int color)
	{
		m_ScreenPixels[pixel_x][pixel_y].SetColor(color);
	}
}