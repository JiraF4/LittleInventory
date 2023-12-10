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
	
	ButtonWidget m_wCloseButton;
	
	override void HandlerAttached(Widget w)
	{
		super.HandlerAttached(w);
		
		if (!GetGame().InPlayMode())
			return;
		
		m_gLittleInventoryEntityItems = GridLayoutWidget.Cast(w.FindAnyWidget("LittleInventoryEntityItems"));
		m_gLittleInventoryEntitySlots = GridLayoutWidget.Cast(w.FindAnyWidget("LittleInventoryEntitySlots"));
		
		m_wEntityName = TextWidget.Cast(w.FindAnyWidget("EntityName"));
		m_wEntityWeight = TextWidget.Cast(w.FindAnyWidget("EntityWeight"));
		m_wEntityVolume = TextWidget.Cast(w.FindAnyWidget("EntityVolume"));
		
		m_wCloseButton = ButtonWidget.Cast(w.FindAnyWidget("CloseButton"));
		SCR_ButtonBaseComponent button = SCR_ButtonBaseComponent.Cast(m_wCloseButton.FindHandler(SCR_ButtonBaseComponent));
		button.m_OnClicked.Insert(OnCloseClick);
	}
	
	void SetInventory(PS_LittleInventory littleInventory)
	{
		m_iLittleInventory = littleInventory;
	}
	
	void SetEntity(IEntity entity)
	{
		InventoryItemComponent inventoryItem = InventoryItemComponent.Cast(entity.FindComponent(InventoryItemComponent));
		BaseInventoryStorageComponent storage = BaseInventoryStorageComponent.Cast(inventoryItem);
		ClothNodeStorageComponent ClothNode = ClothNodeStorageComponent.Cast(inventoryItem);
		ChimeraCharacter character = ChimeraCharacter.Cast(entity);
		
		if (inventoryItem)
		{
			UIInfo info = inventoryItem.GetUIInfo();
			if (info)
			{
				m_wEntityName.SetText(info.GetName());
				m_wEntityWeight.SetText("");
				m_wEntityVolume.SetText("");
			}
		}
		
		array<IEntity> items = new array<IEntity>();
		if (storage) storage.GetAll(items);
		array<ref PS_SlotCell> slots = {};
		
		if (character)
		{
			SCR_InventoryConfig inventoryUIConfig = SCR_InventoryConfig.Cast(BaseContainerTools.CreateInstanceFromContainer(BaseContainerTools.LoadContainer("{024B56A4DE577001}Configs/Inventory/InventoryUI.conf").GetResource().ToBaseContainer()));
			SCR_CharacterInventoryStorageComponent characterInventoryStorage = SCR_CharacterInventoryStorageComponent.Cast(storage);
			
			SCR_EditableCharacterComponent editableCharacter = SCR_EditableCharacterComponent.Cast(character.FindComponent(SCR_EditableCharacterComponent));
			SCR_UIInfo info = editableCharacter.GetInfo();
			m_wEntityName.SetText(info.GetName());
			m_wEntityWeight.SetText("");
			m_wEntityVolume.SetText("");
			
			
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
		
		if (ClothNode)
		{
			int slotsCount = ClothNode.GetSlotsCount();
			for (int i = 0; i < slotsCount; i++)
			{
				InventoryStorageSlot slot = ClothNode.GetSlot(i);
				slots.Insert(new PS_SlotCell(slot.GetAttachedEntity(), ""));
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
	
	void OnCloseClick(SCR_ButtonTextComponent button)
	{
		m_wRoot.RemoveFromHierarchy();
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




































