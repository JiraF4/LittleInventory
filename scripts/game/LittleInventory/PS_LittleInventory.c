class PS_LittleInventory : SCR_ScriptedWidgetComponent 
{
	ResourceName m_rLittleInventoryEntity = "{0E16E87C294C2445}UI/LittleInventory/LittleInventoryEntity.layout";
	
	VerticalLayoutWidget m_vLittleInventoryEntitiesList;
	
	PS_LittleInventoryItemInfo m_hLittleInventoryItemInfo;
	PS_LittleInventoryItemCell m_cLittleInventoryItemCellSelected;
	
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
		
		IEntity entityBTR = GetGame().FindEntity("BTR");
		if (entityBTR)
			OpenEntity(entityBTR);
	};
	
	void OpenEntity(IEntity entity)
	{
		Widget littleInventoryEntityWidget = GetGame().GetWorkspace().CreateWidgets(m_rLittleInventoryEntity, m_vLittleInventoryEntitiesList);
		PS_LittleInventoryEntity littleInventoryEntity = PS_LittleInventoryEntity.Cast(littleInventoryEntityWidget.FindHandler(PS_LittleInventoryEntity));
		littleInventoryEntity.SetInventory(this);
		littleInventoryEntity.SetEntity(entity);
	}
	
	void CellMove(PS_LittleInventoryItemCell cellFrom, PS_LittleInventoryItemCell cellTo)
	{
		IEntity itemEntity = cellFrom.GetItem();
		if (!itemEntity)
			return;
		
		PS_LittleInventoryEntity littleInventoryEntityFrom = cellFrom.GetInventoryEntity();
		PS_LittleInventoryEntity littleInventoryEntityTo = cellTo.GetInventoryEntity();
		
		BaseInventoryStorageComponent storageFrom = cellFrom.GetStorage();
		BaseInventoryStorageComponent storageTo = cellTo.GetStorage();
		
		IEntity entityBorya = GetGame().FindEntity("Borya");
		SCR_InventoryStorageManagerComponent inventoryManager = SCR_InventoryStorageManagerComponent.Cast(entityBorya.FindComponent(SCR_InventoryStorageManagerComponent));
		
		PS_LittleInventoryInventoryOperationCallback cb = new PS_LittleInventoryInventoryOperationCallback();
		
		int slotId = cellTo.GetSlotId();
		
		if (slotId >= 0) inventoryManager.TryMoveItemToStorage(itemEntity, storageTo, slotId, cb);
		else 
		{
			storageTo = inventoryManager.FindStorageForInsert(itemEntity, storageTo, EStoragePurpose.PURPOSE_ANY);
			inventoryManager.TryMoveItemToStorage(itemEntity, storageTo, -1, cb);
		}
		
		littleInventoryEntityFrom.Refresh();
		littleInventoryEntityTo.Refresh();
	}
	
	// events
	void OnCellSelect(PS_LittleInventoryItemCell cell)
	{
		m_cLittleInventoryItemCellSelected = cell;
		
		Widget widget = m_vLittleInventoryEntitiesList.GetChildren();
		while (widget)
		{
			PS_LittleInventoryEntity littleInventoryEntity = PS_LittleInventoryEntity.Cast(widget.FindHandler(PS_LittleInventoryEntity));
			if (cell.GetInventoryEntity() != littleInventoryEntity)
				littleInventoryEntity.ToMoveMode();
			widget = widget.GetSibling();
		}
	}
	void OnCellSelectLost(PS_LittleInventoryItemCell cell)
	{
		if (m_cLittleInventoryItemCellSelected && cell && m_cLittleInventoryItemCellSelected != cell)
		{
			CellMove(m_cLittleInventoryItemCellSelected, cell);
		}
		m_cLittleInventoryItemCellSelected = null;
		
		Widget widget = m_vLittleInventoryEntitiesList.GetChildren();
		while (widget)
		{
			PS_LittleInventoryEntity littleInventoryEntity = PS_LittleInventoryEntity.Cast(widget.FindHandler(PS_LittleInventoryEntity));
			littleInventoryEntity.ResetMoveMode();
			widget = widget.GetSibling();
		}
	}
	
	void OnCellFocus(PS_LittleInventoryItemCell cell)
	{
		m_hLittleInventoryItemInfo.SetCell(cell);
	}
	void OnCellFocusLost(PS_LittleInventoryItemCell cell)
	{
		m_hLittleInventoryItemInfo.SetCell(null);
	}
	
	void OnCellClick(PS_LittleInventoryItemCell cell)
	{
		IEntity item = cell.GetItem();
		if (!item) return;
		InventoryItemComponent inventoryItem = InventoryItemComponent.Cast(item.FindComponent(InventoryItemComponent));
		BaseInventoryStorageComponent storage = BaseInventoryStorageComponent.Cast(inventoryItem);
		if (storage) OpenEntity(item);
	}
}

class PS_LittleInventoryInventoryOperationCallback : ScriptedInventoryOperationCallback
{
	override void OnComplete()
	{
		Print("OnComplete");
	}
	
	override protected void OnFailed()
	{
		Print("OnFailed");
	}
}