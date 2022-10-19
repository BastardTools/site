class Bastard_Build_house extends BaseBuildingBase
{	
	protected bool m_HasGate		= false;
	protected bool m_IsOpened 		= false;
	protected bool m_IsOpenedClient	= false;
	protected bool m_IsOpened_Window 		= false;
	protected bool m_IsOpened_WindowClient	= false;
	
	typename ATTACHMENT_WOODEN_LOG			= WoodenLog;
	typename ATTACHMENT_COMBINATION_LOCK	= CombinationLock;
	
	const string ATTACHMENT_SLOT_COMBINATION_LOCK = "Att_CombinationLock";
	
	const string SOUND_GATE_OPEN_START			= "DoorWoodTowerOpen_SoundSet";
	const string SOUND_GATE_CLOSE_START			= "DoorWoodTowerClose_start_SoundSet";
	const string SOUND_GATE_CLOSE_END			= "DoorWoodTowerClose_end_SoundSet";

	const float GATE_ROTATION_ANGLE_DEG 			= 100;
	const float GATE_ROTATION_TIME_APPROX			= 2000;		//ms
	
	const float MAX_ACTION_DETECTION_ANGLE_RAD 		= 1.3;		//1.3 RAD = ~75 DEG
	const float MAX_ACTION_DETECTION_DISTANCE 		= 2.0;		//meters
	
	protected EffectSound m_SoundGate_Start;
	protected EffectSound m_SoundGate_End;
	
	void Bastard_Build_house()
	{
		//synchronized variables
		RegisterNetSyncVariableBool( "m_HasGate" );
		RegisterNetSyncVariableBool( "m_IsOpened" );
		RegisterNetSyncVariableBool( "m_IsOpened_Window" );
	}
	override bool IsBastard_Build_house()
    {
        return true;
    }

	override string GetConstructionKitType()
	{
		return "Bastard_Build_house_Kit";
	}
	
	bool HasGate()
	{
		return m_HasGate;
	}
	
	void SetGateState( bool has_gate )
	{
		m_HasGate = has_gate;
	}	
	
	void SetOpenedState( bool state )
	{
		m_IsOpened = state;
	}
	
	override bool IsOpened()
	{
		return m_IsOpened;
	}
	
	void Setbastard_windowOpenedState( bool state )
	{
		m_IsOpened_Window = state;
	}
	
	bool Isbastard_windowOpened()
	{
		return m_IsOpened_Window;
	}

	bool IsLocked()
	{
		CombinationLock combination_lock = GetCombinationLock();
		if ( combination_lock && combination_lock.IsLocked() )
		{
			return true;
		}
		return false;
	}
		
	CombinationLock GetCombinationLock()
	{
		CombinationLock combination_lock = CombinationLock.Cast( FindAttachmentBySlotName( ATTACHMENT_SLOT_COMBINATION_LOCK ) );
		return combination_lock;
	}
	
	void HandleDropAttachment(ItemBase item)
	{
		CombinationLock lock;
		if (Class.CastTo(lock,item))
		{
			lock.UnlockServer(null,this);
		}
		else if (item)//generic behaviour
		{
			GetInventory().DropEntity(InventoryMode.SERVER, this, item);
		}
	}
	
	override vector GetKitSpawnPosition()
	{
		if ( MemoryPointExists( "kit_spawn_position" ) )
		{
			vector position;
			position = GetMemoryPointPos( "kit_spawn_position" );
			
			return ModelToWorld( position );
		}		
		
		return GetPosition();
	}	

	override bool CanDisplayAttachmentSlot( string slot_name )
	{
		return true;
	}

	override bool CanDisplayAttachmentCategory( string category_name )
	{
		return true;
	}
	
	override void OnStoreSave( ParamsWriteContext ctx )
	{   
		super.OnStoreSave( ctx );
		
		ctx.Write( m_HasGate );
		ctx.Write( m_IsOpened );
		ctx.Write( m_IsOpened_Window );
		bsbDebugPrint("[bsb] OnStoreSave - build=" + m_HasGate + " opened=" + m_IsOpened);
	}
	
	override bool OnStoreLoad( ParamsReadContext ctx, int version )
	{
		if ( !super.OnStoreLoad( ctx, version ) )
			return false;

		if ( !ctx.Read( m_HasGate ) )
		{
			m_HasGate = false;
			return false;
		}
		
		if ( !ctx.Read( m_IsOpened ) )
		{
			m_IsOpened = false;
			return false;
		}
		
		if ( !ctx.Read( m_IsOpened_Window ) )
		{
			m_IsOpened_Window = false;
			return false;
		}
		
		bsbDebugPrint("[bsb] OnStoreLoad - build=" + m_HasGate + " opened=" + m_IsOpened);
		
		return true;
	}
	
	override void AfterStoreLoad()
	{	
		super.AfterStoreLoad();
		
		//set gate state
		ConstructionPart gate_part = GetConstruction().GetGateConstructionPart();
		SetGateState( gate_part.IsBuilt() );
		
		//update gate state visual
		if ( IsOpened() )
		{
			Openbastard_door();
		}
		else
		{
			Closebastard_door();
		}
		
		if ( Isbastard_windowOpened() )
		{
			Openbastard_window();
		}
		else
		{
			Closebastard_window();
		}
		
		UpdateVisuals();
		
		bsbDebugPrint("[bsb] AfterStoreLoad - build=" + gate_part.IsBuilt() + " opened=" + IsOpened());
	}
	
	override void OnVariablesSynchronized()
	{
		super.OnVariablesSynchronized();

		if ( m_IsOpenedClient != m_IsOpened )
		{
			m_IsOpenedClient = m_IsOpened;
			
			if ( m_IsOpenedClient )
			{
				Openbastard_door();
			}
			else
			{
				Closebastard_door();
			}
		}
		
		if ( m_IsOpened_WindowClient != m_IsOpened_Window )
		{
			m_IsOpened_WindowClient = m_IsOpened_Window;
			
			if ( m_IsOpened_WindowClient )
			{
				Openbastard_window();
			}
			else
			{
				Closebastard_window();
			}
		}
	}	

	override void OnPartBuiltServer( notnull Man player, string part_name, int action_id )
	{
		ConstructionPart constrution_part = GetConstruction().GetConstructionPart( part_name );
		
		if ( constrution_part.IsBase() )
		{
			SetBaseState( true );
			
			//hide hologram
			float value = 1;
			SetAnimationPhase( "Hologram", 		value );
			
			//spawn toolkit
			//CreateConstructionKit();
		}
		
		//check gate state
		if ( constrution_part.IsGate() )
		{
			SetGateState( true );
		}
		
		super.OnPartBuiltServer( player, part_name, action_id );
		//update visuals (server)
		UpdateVisuals();
	}
	
	override void OnPartDismantledServer( notnull Man player, string part_name, int action_id )
	{
		ConstructionPart constrution_part = GetConstruction().GetConstructionPart( part_name );

		//check gate state
		if ( constrution_part.IsGate() )
		{
			SetGateState( false );
		}
		
		//check gate state
		if ( constrution_part.IsGate() )
		{
			if ( IsLocked() )
			{
				CombinationLock combination_lock = CombinationLock.Cast( FindAttachmentBySlotName( ATTACHMENT_SLOT_COMBINATION_LOCK ) );
				combination_lock.UnlockServer( player , this );
			}
		}
		
		super.OnPartDismantledServer( player, part_name, action_id );
	}
	
	override void OnPartDestroyedServer( Man player, string part_name, int action_id,  bool destroyed_by_connected_part = false )
	{
		ConstructionPart constrution_part = GetConstruction().GetConstructionPart( part_name );

		if ( constrution_part.IsGate() )
		{
			SetGateState( false );
		}

		super.OnPartDestroyedServer( player, part_name, action_id );
	}

	override bool CanReceiveAttachment( EntityAI attachment, int slotId )
	{
		//manage action initiator (AT_ATTACH_TO_CONSTRUCTION)
		if ( !GetGame().IsMultiplayer() || GetGame().IsClient() )
		{
			PlayerBase player = PlayerBase.Cast( GetGame().GetPlayer() );
			if ( player )
			{
				ConstructionActionData construction_action_data = player.GetConstructionActionData();
				
				//reset action initiator
				construction_action_data.SetActionInitiator( NULL );				
			}
		}
		
		if ( attachment.Type() == ATTACHMENT_COMBINATION_LOCK )
		{
			if ( !HasGate() || IsOpened() )
			{
				return false;
			}
		}
			
		return true;
	}
	
	override bool CanPutIntoHands( EntityAI parent )
	{
		if( !super.CanPutIntoHands( parent ) )
		{
			return false;
		}
		
		if ( HasBase() )
		{
			return false;
		}
		
		return true;
	}
	
	bool CanOpenbastard_door()
	{
		if ( HasGate() && !IsOpened() && !IsLocked() )
		{
			return true;
		}
		
		return false;
	}
	
	bool CanClosebastard_door()
	{
		if ( HasGate() && IsOpened() )
		{
			return true;
		}
		
		return false;
	}
	
	void Openbastard_door()
	{
		//server or single player
		if ( GetGame().IsServer() )
		{
			float value = GATE_ROTATION_ANGLE_DEG;
			SetAnimationPhase( "bastard_door_Interact_Rotate", 	value );
			SetAnimationPhase( "bastard_door_Rotate", 				value );
			
			SetOpenedState( true );
			
			//regenerate navmesh
			GetGame().GetCallQueue( CALL_CATEGORY_GAMEPLAY ).CallLater( UpdateNavmesh, GATE_ROTATION_TIME_APPROX, false );
			
			//synchronize
			SynchronizeBaseState();
		}
		
		//client or single player
		if ( !GetGame().IsMultiplayer() || GetGame().IsClient() )
		{
			//play sound
			SoundGateOpenStart();
		}
		
		//add check
		GetGame().GetCallQueue( CALL_CATEGORY_GAMEPLAY ).Remove(Checkbastard_doorClosed);
		GetGame().GetCallQueue( CALL_CATEGORY_GAMEPLAY ).CallLater( Checkbastard_doorOpened, 0, true );
	}
	
	void Closebastard_door()
	{
		//server or single player
		if ( GetGame().IsServer() )
		{		
			float value = 0;
			SetAnimationPhase( "bastard_door_Interact_Rotate", 	value );
			SetAnimationPhase( "bastard_door_Rotate", 				value );

			SetOpenedState( false );
			
			//regenerate navmesh
			GetGame().GetCallQueue( CALL_CATEGORY_GAMEPLAY ).CallLater( UpdateNavmesh, GATE_ROTATION_TIME_APPROX, false );
			
			//synchronize
			SynchronizeBaseState();
		}
		
		//client or single player
		if ( !GetGame().IsMultiplayer() || GetGame().IsClient() )
		{
			//play sound
			SoundGateCloseStart();
		}
		
		//add check
		GetGame().GetCallQueue( CALL_CATEGORY_GAMEPLAY ).Remove(Checkbastard_doorOpened);
		GetGame().GetCallQueue( CALL_CATEGORY_GAMEPLAY ).CallLater( Checkbastard_doorClosed, 0, true );
	}
	
	protected void Checkbastard_doorClosed()
	{
		if ( GetAnimationPhase( "bastard_door_Rotate" ) == 0 )			//animation closed
		{
			//play sound
			if ( this ) SoundGateCloseEnd();
			
			//remove check
			GetGame().GetCallQueue( CALL_CATEGORY_GAMEPLAY ).Remove( Checkbastard_doorClosed );
		}
	}
	
	protected void Checkbastard_doorOpened()
	{
		if ( GetAnimationPhase( "bastard_door_Rotate" ) == GATE_ROTATION_ANGLE_DEG )			//animation finished - open
		{
			//remove check
			GetGame().GetCallQueue( CALL_CATEGORY_GAMEPLAY ).Remove( Checkbastard_doorOpened );
		}
	}
	
	bool CanOpenbastard_window()
	{
		if ( HasGate() && !Isbastard_windowOpened() && !IsLocked() )
		{
			return true;
		}
		
		return false;
		UpdateVisuals();
	}
	
	bool CanClosebastard_window()
	{
		if ( HasGate() && Isbastard_windowOpened() )
		{
			return true;
		}
		
		return false;
		UpdateVisuals();
	}
	
	void Openbastard_window()
	{
		//server or single player
		if ( GetGame().IsServer() )
		{
			float value = GATE_ROTATION_ANGLE_DEG;
			SetAnimationPhase( "bastard_window_Interact_Rotate", 		value );
			SetAnimationPhase( "bastard_window_Rotate", 				value );
			
			Setbastard_windowOpenedState( true );
			
			//regenerate navmesh
			GetGame().GetCallQueue( CALL_CATEGORY_GAMEPLAY ).CallLater( UpdateNavmesh, GATE_ROTATION_TIME_APPROX, false );
			
			//synchronize
			SynchronizeBaseState();
		}
		
		//client or single player
		if ( !GetGame().IsMultiplayer() || GetGame().IsClient() )
		{
			//play sound
			SoundGateOpenStart();
		}
		
		//add check
		GetGame().GetCallQueue( CALL_CATEGORY_GAMEPLAY ).Remove(Checkbastard_windowClosed);
		GetGame().GetCallQueue( CALL_CATEGORY_GAMEPLAY ).CallLater( Checkbastard_windowOpened, 0, true );
	}
	
	void Closebastard_window()
	{
		//server or single player
		if ( GetGame().IsServer() )
		{		
			float value = 0;
			SetAnimationPhase( "bastard_window_Interact_Rotate", 		value );
			SetAnimationPhase( "bastard_window_Rotate", 				value );

			Setbastard_windowOpenedState( false );
			
			//regenerate navmesh
			GetGame().GetCallQueue( CALL_CATEGORY_GAMEPLAY ).CallLater( UpdateNavmesh, GATE_ROTATION_TIME_APPROX, false );
			
			//synchronize
			SynchronizeBaseState();
		}
		
		//client or single player
		if ( !GetGame().IsMultiplayer() || GetGame().IsClient() )
		{
			//play sound
			SoundGateCloseStart();
		}
		
		//add check
		GetGame().GetCallQueue( CALL_CATEGORY_GAMEPLAY ).Remove(Checkbastard_windowOpened);
		GetGame().GetCallQueue( CALL_CATEGORY_GAMEPLAY ).CallLater( Checkbastard_windowClosed, 0, true );
	}
	
	protected void Checkbastard_windowClosed()
	{
		if ( GetAnimationPhase( "bastard_window_Rotate" ) == 0 )			//animation closed
		{
			//remove check
			GetGame().GetCallQueue( CALL_CATEGORY_GAMEPLAY ).Remove( Checkbastard_windowClosed );
		}
	}
	
	protected void Checkbastard_windowOpened()
	{
		if ( GetAnimationPhase( "bastard_window_Rotate" ) == GATE_ROTATION_ANGLE_DEG )			//animation finished - open
		{
			//remove check
			GetGame().GetCallQueue( CALL_CATEGORY_GAMEPLAY ).Remove( Checkbastard_windowOpened );
		}
	}
	
	void UpdateVisualState()
    {
        if ( IsOpen() )
        {
			SetAnimationPhase("bastard_door",1);
        }
        else
        {
			SetAnimationPhase("bastard_door",0);
        }
		
        if ( Isbastard_windowOpened() )
        {
            SetAnimationPhase("bastard_window",1);
        }
        else
        {
            SetAnimationPhase("bastard_window",0);
        }		
    }

	override void CreateAreaDamage( string slot_name, float rotation_angle = 0 )
	{
		if ( IsOpened() )
		{
			rotation_angle = 0;
		}
		
		super.CreateAreaDamage( slot_name, rotation_angle );
	}	
	
	override bool IsFacingPlayer( PlayerBase player, string selection )
	{
		return false;
	}
	
	override bool IsFacingCamera( string selection )
	{
		return false;
	}
	
	override bool HasProperDistance( string selection, PlayerBase player )
	{
		if ( MemoryPointExists( selection ) )
		{
			vector selection_pos = ModelToWorld( GetMemoryPointPos( selection ) );
			float distance = vector.Distance( selection_pos, player.GetPosition() );
			if ( distance >= MAX_ACTION_DETECTION_DISTANCE )
			{
				return false;
			}
		}
		
		return true;
	}
	
	protected void SoundGateOpenStart()
	{
		PlaySoundSet( m_SoundGate_Start, SOUND_GATE_OPEN_START, 0.1, 0.1 );
	}

	protected void SoundGateCloseStart()
	{
		PlaySoundSet( m_SoundGate_Start, SOUND_GATE_CLOSE_START, 0.1, 0.1 );
	}

	protected void SoundGateCloseEnd()
	{
		PlaySoundSet( m_SoundGate_End, SOUND_GATE_CLOSE_END, 0.1, 0.1 );
	}
	
	override void SetActions()
	{
		super.SetActions();
		
		AddAction(ActionTogglePlaceObject);
		AddAction(ActionPlaceObject);
		AddAction(ActionFoldBaseBuildingObject);
		AddAction(ActionDialCombinationLockOnbastard_door);
		AddAction(ActionNextCombinationLockDialOnbastard_door);
		AddAction(ActionOpenbastard_door);
		AddAction(ActionOpenbastard_window);
		AddAction(ActionClosebastard_door);
		AddAction(ActionClosebastard_window);
    }
}
