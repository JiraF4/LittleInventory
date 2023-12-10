class PS_LittleInventory : SCR_ScriptedWidgetComponent 
{
	ResourceName m_rLittleInventoryEntity = "{0E16E87C294C2445}UI/LittleInventory/LittleInventoryEntity.layout";
	
	VerticalLayoutWidget m_vLittleInventoryEntitiesList;
	
	PS_LittleInventoryItemInfo m_hLittleInventoryItemInfo;
	
	override void HandlerAttached(Widget w)
	{
		super.HandlerAttached(w);
		m_vLittleInventoryEntitiesList = VerticalLayoutWidget.Cast(w.FindAnyWidget("LittleInventoryEntitiesList"));
		m_hLittleInventoryItemInfo = PS_LittleInventoryItemInfo.Cast(w.FindAnyWidget("LittleInventoryItemInfo").FindHandler(PS_LittleInventoryItemInfo));
		
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
		m_hLittleInventoryItemInfo.SetCell(cell);
		IEntity item = cell.GetItem();
		if (!item) return;
		InventoryItemComponent inventoryItem = InventoryItemComponent.Cast(item.FindComponent(InventoryItemComponent));
		BaseInventoryStorageComponent storage = BaseInventoryStorageComponent.Cast(inventoryItem);
		if (storage) OpenEntity(item);
	}
}