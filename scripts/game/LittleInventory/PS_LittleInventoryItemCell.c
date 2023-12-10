class PS_LittleInventoryItemCell : SCR_ScriptedWidgetComponent
{
	ItemPreviewWidget m_iItemPreview;
	IEntity m_eItemEntity;
	
	PS_LittleInventoryEntity m_iLittleInventoryEntity;
	
	SCR_ButtonTextComponent m_bButton;
	
	override void HandlerAttached(Widget w)
	{
		m_iItemPreview = ItemPreviewWidget.Cast(w.FindAnyWidget("ItemPreview"));
		m_bButton = SCR_ButtonTextComponent.Cast(w.FindAnyWidget("Button").FindHandler(SCR_ButtonTextComponent));
		m_bButton.m_OnClicked.Insert(OnCellClick);
		super.HandlerAttached(w);
	}
	
	void SetInventoryEntity(PS_LittleInventoryEntity littleInventoryEntity)
	{
		m_iLittleInventoryEntity = littleInventoryEntity;
	}
	
	void SetItem(IEntity itemEntity)
	{
		m_eItemEntity = itemEntity;
		
		ChimeraWorld world = ChimeraWorld.CastFrom(GetGame().GetWorld());
		ItemPreviewManagerEntity previewManager = world.GetItemPreviewManager();
		if (!previewManager)
		{
			Resource rsc = Resource.Load("{9F18C476AB860F3B}Prefabs/World/Game/ItemPreviewManager.et");
			if (rsc.IsValid())
				GetGame().SpawnEntityPrefabLocal(rsc, world);
			previewManager = world.GetItemPreviewManager();
		}
		previewManager.SetPreviewItem(m_iItemPreview, itemEntity);
	}
	
	IEntity GetItem()
	{
		return m_eItemEntity;
	}
	
	void OnCellClick(SCR_ButtonTextComponent button)
	{
		m_iLittleInventoryEntity.OnCellClick(this);
	}
}