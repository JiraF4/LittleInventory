class PS_LittleInventory : SCR_ScriptedWidgetComponent 
{
	ResourceName m_rLittleInventoryEntity = "{0E16E87C294C2445}UI/LittleInventory/LittleInventoryEntity.layout";
	
	VerticalLayoutWidget m_vLittleInventoryEntitiesList;
	
	override void HandlerAttached(Widget w)
	{
		super.HandlerAttached(w);
		m_vLittleInventoryEntitiesList = VerticalLayoutWidget.Cast(w.FindAnyWidget("LittleInventoryEntitiesList"));
		
		GetGame().GetCallqueue().CallLater(TestOpen, 0);
	}
	
	void TestOpen()
	{
		IEntity entity = GetGame().FindEntity("Borya");
		if (entity)
			OpenEntity(entity);
	};
	
	void OpenEntity(IEntity entity)
	{
		Widget littleInventoryEntityWidget = GetGame().GetWorkspace().CreateWidgets(m_rLittleInventoryEntity, m_vLittleInventoryEntitiesList);
		PS_LittleInventoryEntity littleInventoryEntity = PS_LittleInventoryEntity.Cast(littleInventoryEntityWidget.FindHandler(PS_LittleInventoryEntity));
		littleInventoryEntity.SetInventory(this);
		littleInventoryEntity.SetEntity(entity);
	}
	
	void OnCellClick(PS_LittleInventoryItemCell cell)
	{
		IEntity item = cell.GetItem();
		OpenEntity(item);
	}
}