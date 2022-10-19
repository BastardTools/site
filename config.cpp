class CfgPatches
{
	class Bastard_Build_house
	{
		units[] = {"Bastard_Build_house_Kit","bastard_hologram","Bastard_Build_house","Bastard_Build_Menu"};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Characters","DZ_Characters_Backpacks","DZ_Characters_Pants","DZ_Characters_Tops","DZ_Gear_Containers","DZ_Characters_Headgear","DZ_Gear_Camping","DZ_Gear_Crafting","DZ_Gear_Tools","DZ_Animals","DZ_Data", "Bastard_Material"};
	};
};
class CfgMods 
{
	class bastard
	{
		dir = "bastard";
		name = "Bastard Build Script";
		author = "bastard.info";
		type = "mod";
		dependencies[] = {"Game","World","Mission","gui"};

		class defs 
		{
			class worldScriptModule
			{
				value = "";
				files[] = {"Bastard_Build\bastard_house\scripts\4_World"};
			};
		};
	};
};
class CfgVehicles
{
	class Inventory_Base;
	class FenceKit;
	class Bastard_Build_Menu: Inventory_Base
	{
		scope = 2;
		displayName = "Макет Маленького Дома";
		descriptionShort = "Макет Маленького Дома";
		model = "\Bastard_Build\bastard_house\Bastard_Build_Menu.p3d";
	};
	class Bastard_Build_house_Kit: FenceKit
	{
		scope = 2;
		displayName = "Набор для постройки Маленького Дома";
		descriptionShort = "Набор для постройки Маленького Дома";
		model = "\Bastard_Build\bastard_house\Bastard_Build_house_kit.p3d";
		reversed = 1;
		minPlacingDist = 10;
		class AnimationSources
		{
			class AnimSourceShown
			{
				source = "user";
				animPeriod = 0.01;
				initPhase = 0;
			};
			class AnimSourceHidden
			{
				source = "user";
				animPeriod = 0.01;
				initPhase = 1;
			};
			class Inventory: AnimSourceHidden{};
			class Placing: AnimSourceHidden{};
		};
	};
	class bastard_hologram: Bastard_Build_house_Kit
	{
		scope = 2;
		model = "\Bastard_Build\bastard_house\bastard_hologram.p3d";
		storageCategory = 10;
		hiddenSelections[] = {"placing"};
		alignHologramToTerain = 1;
		slopeTolerance = 0.3;
	};
	class Bastard_Build_house: Inventory_Base
	{
		scope = 2;
		//displayName = "Маленький Дом";
		//descriptionShort = "Маленький Дом";
		model = "\Bastard_Build\bastard_house\Bastard_Build_house.p3d";
		overrideDrawArea = "4.0";
		forceFarBubble = "true";
		lootCategory = "Crafted";
		carveNavmesh = 1;
		weight = 60000;
		itemSize[] = {6,6};
		physLayer = "item_large";
		createProxyPhysicsOnInit = "false";
		createdProxiesOnInit[] = {"Deployed"};
		rotationFlags = 0;
		bounding = "bounding";
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 100;
				};
			};
			class GlobalArmor
			{
				class Projectile
				{
					class Health
					{
						damage = 0;
					};
					class Blood
					{
						damage = 0;
					};
					class Shock
					{
						damage = 0;
					};
				};
				class FragGrenade
				{
					class Health
					{
						damage = 0;
					};
					class Blood
					{
						damage = 0;
					};
					class Shock
					{
						damage = 0;
					};
				};
			};
		};
		attachments[] = {"Att_CombinationLock","Material_Nails","Truck_01_WoodenLogs","Material_CM_WoodenPlanks","Stones","Material_L1_WoodenLogs","Material_L1_WoodenPlanks","Material_L1_Nails","bastard_CementBag"};
		class GUIInventoryAttachmentsProps
		{
			class Bastard_Build_house_Attachments
			{
				name = "Attachments";
				description = "";
				attachmentSlots[] = {"Att_CombinationLock"};
				icon = "cat_bb_attachments";
				selection = "bastard_door";
			};
			class Bastard_Build_house_Frame
			{
				name = "Materials";
				description = "";
				attachmentSlots[] = {"Material_Nails","Truck_01_WoodenLogs","Material_CM_WoodenPlanks","Stones", "bastard_CementBag"};
				icon = "cat_bb_material";
				selection = "bobthebuilder";
			};
		};
		class AnimationSources
		{
			class AnimSourceShown
			{
				source = "user";
				animPeriod = 0.01;
				initPhase = 0;
			};
			class AnimSourceHidden
			{
				source = "user";
				animPeriod = 0.01;
				initPhase = 1;
			};
			class AnimRotate
			{
				source = "user";
				animPeriod = 0.01;
				initPhase = 0;
			};
			class Material_Nails: AnimSourceHidden{};
			class bastard_CementBag: AnimSourceHidden{};
			class Truck_01_WoodenLogs: AnimSourceHidden{};
			class Material_CM_WoodenPlanks: AnimSourceHidden{};
			class Material_L1_Nails: AnimSourceHidden{};
			class Material_L1_WoodenLogs: AnimSourceHidden{};
			class Material_L1_WoodenPlanks: AnimSourceHidden{};
			class Stones: AnimSourceHidden{};
			
			class Deployed: AnimSourceHidden{};
			class Hologram: AnimSourceHidden{};
			
			class bastard_hologram: AnimSourceHidden{};
			class bastard_foundation: AnimSourceHidden{};
			class bastard_wall_1: AnimSourceHidden{};
			class bastard_wall_2: AnimSourceHidden{};
			class bastard_wall_3: AnimSourceHidden{};
			
			class bastard_door: AnimSourceHidden{};
			class bastard_door_Interact_Rotate: AnimRotate{};
			class bastard_door_Rotate: AnimRotate{};
			
			class bastard_window: AnimSourceHidden{};
			class bastard_window_Interact_Rotate: AnimRotate{};
			class bastard_window_Rotate: AnimRotate{};
		};
		class Construction
		{
			class bastardhouse
			{
				class bastard_foundation
				{
					name = "Построить Фундамент";
					is_base = 1;
					id = 1;
					required_parts[] = {};
					conflicted_parts[] = {};
					collision_data[] = {};
					build_action_type = 4;
					dismantle_action_type = 4;
					material_type = 1;
					class Materials
					{
						class Material1
						{
							type = "Stone";
							slot_name = "Stones";
							quantity = 40;
						};
						class Material2
						{
							type = "bastard_CementBag";
							slot_name = "bastard_CementBag";
							quantity = 40;
						};
					};
				};
				class bastard_wall_1
				{
					name = "Построить 1 этап";
					id = 2;
					required_parts[] = {"bastard_foundation"};
					conflicted_parts[] = {};
					collision_data[] = {};
					build_action_type = 2;
					dismantle_action_type = 2;
					material_type = 2;
					class Materials
					{
						class Material1
						{
							type = "Nail";
							slot_name = "Material_Nails";
							quantity = 99;
						};
						class Material2
						{
							type = "WoodenLog";
							slot_name = "Truck_01_WoodenLogs";
							quantity = 20;
						};
						class Material3
						{
							type = "WoodenPlank";
							slot_name = "Material_CM_WoodenPlanks";
							quantity = 50;
						};
						class Material4
						{
							type = "Stone";
							slot_name = "Stones";
							quantity = 40;
						};
					};
				};
				class bastard_wall_2
				{
					name = "Построить 2 этап";
					id = 3;
					required_parts[] = {"bastard_wall_1"};
					conflicted_parts[] = {};
					collision_data[] = {};
					build_action_type = 2;
					dismantle_action_type = 2;
					material_type = 2;
					class Materials
					{
						class Material1
						{
							type = "Nail";
							slot_name = "Material_Nails";
							quantity = 99;
						};
						class Material2
						{
							type = "WoodenLog";
							slot_name = "Truck_01_WoodenLogs";
							quantity = 50;
						};
						class Material3
						{
							type = "WoodenPlank";
							slot_name = "Material_CM_WoodenPlanks";
							quantity = 20;
						};
					};
				};
				class bastard_wall_3
				{
					name = "Построить 3 этап";
					id = 4;
					required_parts[] = {"bastard_wall_2"};
					conflicted_parts[] = {};
					collision_data[] = {};
					build_action_type = 2;
					dismantle_action_type = 2;
					material_type = 2;
					class Materials
					{
						class Material1
						{
							type = "Nail";
							slot_name = "Material_Nails";
							quantity = 99;
						};
						class Material2
						{
							type = "WoodenLog";
							slot_name = "Truck_01_WoodenLogs";
							quantity = 20;
						};
						class Material3
						{
							type = "WoodenPlank";
							slot_name = "Material_CM_WoodenPlanks";
							quantity = 50;
						};
						class Material4
						{
							type = "bastard_CementBag";
							slot_name = "bastard_CementBag";
							quantity = 40;
						};
					};
				};
				class bastard_door
				{
					name = "Построить Дверь";
					is_gate = 1;
					id = 5;
					required_parts[] = {"bastard_wall_3"};
					conflicted_parts[] = {};
					collision_data[] = {};
					build_action_type = 2;
					dismantle_action_type = 2;
					material_type = 2;
					class Materials
					{
						class Material1
						{
							type = "Nail";
							slot_name = "Material_Nails";
							quantity = 33;
						};
						class Material2
						{
							type = "WoodenPlank";
							slot_name = "Material_CM_WoodenPlanks";
							quantity = 30;
						};
					};
				};
			};
		};
	};
};