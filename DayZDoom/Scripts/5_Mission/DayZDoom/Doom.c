typedef array<Widget> PixelArray;

class Doom
{
	// static constructor style
	// cause i really want to type Doom.Run(); lol
	private static ref Doom m_Instance;
	
	static void Run()
	{
		if (GetGame().IsServer()) {
			return;
		}
		
		if (m_Instance) {
			delete m_Instance;
		}
		
		m_Instance = new Doom();
		
	}
	
	static Doom GetInstance()
	{
		return m_Instance;
	}
	
	static const int SCREEN_WIDTH = 350;
	static const int SCREEN_HEIGHT = 250;
	
	protected Widget m_LayoutRoot;
	protected ref array<ref PixelArray> m_ScreenPixels = new array<ref PixelArray>();
	
	private void Doom()
	{
		m_LayoutRoot = GetGame().GetWorkspace().CreateWidgets("DayZDoom/GUI/layouts/panel_display.layout");
		Print(m_LayoutRoot);
		for (int i = 0; i < SCREEN_WIDTH; i++) {
			for (int j = 0; i < SCREEN_HEIGHT; j++) {
				m_ScreenPixels[i][j] = GetGame().GetWorkspace().CreateWidget(FrameWidgetTypeID, i, j, 1, 1, WidgetFlags.VISIBLE | WidgetFlags.IGNOREPOINTER, COLOR_YELLOW, 0, m_LayoutRoot);
				Print(m_ScreenPixels[i][j]);
				//SetPixelColor(i, j, COLOR_WHITE);
			}
		}
	}
	
	void SetPixelColor(int pixel_x, int pixel_y, int color)
	{
		m_ScreenPixels[pixel_x][pixel_y].SetColor(color);
	}
}