typedef array<ref Widget> PixelArray;

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
		delete m_Instance;
	}
	
	static Doom GetInstance()
	{
		return m_Instance;
	}
	
	static const int SCREEN_WIDTH = 50;
	static const int SCREEN_HEIGHT = 20;
	static const int PIXEL_SIZE = 10;
	static const int FRAMERATE = 20;
	
	protected Widget m_LayoutRoot;
	protected ref array<ref PixelArray> m_ScreenPixels = {};
	protected ref Timer m_FrameTimer = new Timer(CALL_CATEGORY_SYSTEM);
	
	private void Doom()
	{
		typename pixels = PixelArray;
		m_LayoutRoot = GetGame().GetWorkspace().CreateWidgets("DayZDoom/GUI/layouts/panel_display.layout");
		for (int i = 0; i < SCREEN_WIDTH; i++) {
			if (!m_ScreenPixels[i]) {
				m_ScreenPixels.InsertAt(new PixelArray(), i);
			}
			
			for (int j = 0; j < SCREEN_HEIGHT; j++) {
				//Widget pixel = GetGame().GetWorkspace().CreateWidget(PanelWidgetTypeID, i * PIXEL_SIZE, j * PIXEL_SIZE, PIXEL_SIZE, PIXEL_SIZE, WidgetFlags.EXACTPOS | WidgetFlags.EXACTSIZE | WidgetFlags.VISIBLE | WidgetFlags.IGNOREPOINTER, COLOR_YELLOW, 0, m_LayoutRoot);
				Widget pixel = GetGame().GetWorkspace().CreateWidgets("DayZDoom/GUI/layouts/panel_pixel.layout", m_LayoutRoot);
				pixel.SetPos(i * PIXEL_SIZE, j * PIXEL_SIZE);
				pixel.SetColor(COLOR_RED);
				m_ScreenPixels[i].InsertAt(pixel, j);
			}
		}
		
		m_FrameTimer.Run(1 / FRAMERATE, this, "OnFrame", null, true);
	}
	
	void ~Doom()
	{
		m_FrameTimer.Stop();
		delete m_FrameTimer;
		
		for (int i = 0; i < SCREEN_WIDTH; i++) {
			for (int j = 0; j < SCREEN_HEIGHT; j++) {
				if (m_ScreenPixels[i][j]) {
					m_ScreenPixels[i][j].Unlink();
					delete m_ScreenPixels[i][j];
				}
			}
		}
		
		delete m_ScreenPixels;
		
		if (m_LayoutRoot) {
			m_LayoutRoot.Unlink();
			delete m_LayoutRoot;	
		}
	}
	
	void OnFrame()
	{
		vector dir = GetGame().GetCurrentCameraDirection();
		dir.Normalize();
		float dir1 = dir[1];
		for (int i = 0; i < SCREEN_WIDTH; i++) {
			for (int j = 0; j < SCREEN_HEIGHT; j++) {
				SetPixelColor(i, j, ARGB(1, 1, dir1, 1));	
			}
		}
	}
	
	void SetPixelColor(int pixel_x, int pixel_y, int color)
	{
		m_ScreenPixels[pixel_x][pixel_y].SetColor(color);
	}
}