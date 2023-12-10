class PS_LittleInventoryEntity : SCR_ScriptedWidgetComponent 
{
	ResourceName m_rInventoryIUConfig = "{024B56A4DE577001}Configs/Inventory/InventoryUI.conf";
	
	ResourceName m_rItemCell = "{28561DA61E961875}UI/LittleInventory/LittleInventoryItemCell.layout";
	ResourceName m_rSlotCell = "{297ED2A11D523941}UI/LittleInventory/LittleInventorySlotCell.layout";
	
	PS_LittleInventory m_iLittleInventory
	
	GridLayoutWidget m_gLittleInventoryEntityItems;
	GridLayoutWidget m_gLittleInventoryEntitySlots;
	
	TextWidget m_wEntityName;
	TextWidget m_wEntityWeight;
	TextWidget m_wEntityVolume;
	
	override void HandlerAttached(Widget w)
	{
		super.HandlerAttached(w);
		m_gLittleInventoryEntityItems = GridLayoutWidget.Cast(w.FindAnyWidget("LittleInventoryEntityItems"));
		m_gLittleInventoryEntitySlots = GridLayoutWidget.Cast(w.FindAnyWidget("LittleInventoryEntitySlots"));
		
		m_wEntityName = TextWidget.Cast(w.FindAnyWidget("EntityName"));
		m_wEntityWeight = TextWidget.Cast(w.FindAnyWidget("EntityWeight"));
		m_wEntityVolume = TextWidget.Cast(w.FindAnyWidget("EntityVolume"));
	}
	
	void SetInventory(PS_LittleInventory littleInventory)
	{
		m_iLittleInventory = littleInventory;
	}
	
	void SetEntity(IEntity entity)
	{
		InventoryItemComponent inventoryItem = InventoryItemComponent.Cast(entity.FindComponent(InventoryItemComponent));
		BaseInventoryStorageComponent storage = BaseInventoryStorageComponent.Cast(inventoryItem);
		ChimeraCharacter character = ChimeraCharacter.Cast(entity);
		
		UIInfo info = inventoryItem.GetUIInfo();
		
		
		array<IEntity> items = new array<IEntity>();
		if (storage) storage.GetAll(items);
		
		array<ref PS_SlotCell> slots = {};
		if (character)
		{
			SCR_InventoryConfig inventoryUIConfig = SCR_InventoryConfig.Cast(BaseContainerTools.CreateInstanceFromContainer(BaseContainerTools.LoadContainer("{024B56A4DE577001}Configs/Inventory/InventoryUI.conf").GetResource().ToBaseContainer()));
			SCR_CharacterInventoryStorageComponent characterInventoryStorage = SCR_CharacterInventoryStorageComponent.Cast(storage);
			
			int i = 0;
			LoadoutAreaType loadoutAreaType = inventoryUIConfig.GetAreaByRow(i);
			while (loadoutAreaType)
			{
				IEntity slotItem = null;
				ResourceName slotIcon = "";
				
				slotIcon = inventoryUIConfig.GetIconByRow(i);
				foreach (IEntity item : items)
				{
					BaseLoadoutClothComponent cloth = BaseLoadoutClothComponent.Cast(item.FindComponent(BaseLoadoutClothComponent));
 					int r = inventoryUIConfig.GetRowByArea(cloth.GetAreaType());
					if (r == i)
					{
						slotItem = item;
						break;
					}
				}
				
				slots.Insert(new PS_SlotCell(slotItem, slotIcon));
				
				i++;
				loadoutAreaType = inventoryUIConfig.GetAreaByRow(i);
			}
		}
		
		FillSlotsGrid(slots);
		
		foreach (PS_SlotCell slot : slots)
		{
			// Remove items in slots
			if (slot.m_eItem)
				items.RemoveItem(slot.m_eItem);
		}
		
		FillItemsGrid(items);
	}
	
	void FillSlotsGrid(array<ref PS_SlotCell> slots)
	{
		int x = 0;
		int y = 0;
		foreach (PS_SlotCell slot : slots)
		{
			Widget ItemCell = GetGame().GetWorkspace().CreateWidgets(m_rSlotCell, m_gLittleInventoryEntitySlots);
			GridSlot.SetColumn(ItemCell, x);
			GridSlot.SetRow(ItemCell, y);
			
			PS_LittleInventorySlotCell littleInventorySlotCell = PS_LittleInventorySlotCell.Cast(ItemCell.FindHandler(PS_LittleInventorySlotCell));
			littleInventorySlotCell.SetInventoryEntity(this);
			littleInventorySlotCell.SetSlot(slot);
			
			x++;
			if (x >= 7)
			{
				x = 0;
				y++;
			}
		}
	}
	
	void FillItemsGrid(array<IEntity> items)
	{
		int x = 0;
		int y = 0;
		foreach (IEntity item : items)
		{
			Widget ItemCell = GetGame().GetWorkspace().CreateWidgets(m_rItemCell, m_gLittleInventoryEntityItems);
			GridSlot.SetColumn(ItemCell, x);
			GridSlot.SetRow(ItemCell, y);
			
			PS_LittleInventoryItemCell littleInventorySlotCell = PS_LittleInventoryItemCell.Cast(ItemCell.FindHandler(PS_LittleInventoryItemCell));
			littleInventorySlotCell.SetInventoryEntity(this);
			littleInventorySlotCell.SetItem(item);
			
			x++;
			if (x >= 7)
			{
				x = 0;
				y++;
			}
		}
	}
	
	void OnCellClick(PS_LittleInventoryItemCell cell)
	{
		m_iLittleInventory.OnCellClick(cell)
	}
}

class PS_SlotCell
{
	IEntity m_eItem;
	ResourceName m_rSlotImage;
		
	void PS_SlotCell(IEntity item, ResourceName slotImage)
	{
		m_eItem = item;
		m_rSlotImage = slotImage;
	}
	
	IEntity GetItem()
	{
		return m_eItem;
	}
	
	ResourceName GetImage()
	{
		return m_rSlotImage;
	}
}




































