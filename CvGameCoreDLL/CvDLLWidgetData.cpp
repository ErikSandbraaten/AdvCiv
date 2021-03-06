#include "CvGameCoreDLL.h"
#include "CvDLLWidgetData.h"
#include "CvGamePlay.h"
#include "CvMap.h"
#include "CyArgsList.h"
#include "CvGameTextMgr.h"
#include "CvEventReporter.h"
#include "CvPopupInfo.h"
#include "CvMessageControl.h"
#include "CvBugOptions.h"
#include "AI_Defines.h" // BBAI
#include "WarEvaluator.h" // advc.104l
#include "RiseFall.h" // advc.706
#include "CvDLLInterfaceIFaceBase.h"
#include "CvDLLPythonIFaceBase.h"
#include "CvDLLEngineIFaceBase.h"

CvDLLWidgetData* CvDLLWidgetData::m_pInst = NULL;

CvDLLWidgetData& CvDLLWidgetData::getInstance()
{
	if (m_pInst == NULL)
	{
		m_pInst = new CvDLLWidgetData;
	}
	return *m_pInst;
}

void CvDLLWidgetData::freeInstance()
{
	delete m_pInst;
	m_pInst = NULL;
}

void CvDLLWidgetData::parseHelp(CvWStringBuffer &szBuffer, CvWidgetDataStruct &widgetDataStruct)
{
	// <advc.085> Replacing a few sporadic tests in the parse... functions
	static WidgetTypes aePlayerAsData1[] = { 
		WIDGET_HELP_FINANCE_AWAY_SUPPLY, WIDGET_HELP_FINANCE_CITY_MAINT,
		WIDGET_HELP_FINANCE_CIVIC_UPKEEP, WIDGET_HELP_FINANCE_DOMESTIC_TRADE,
		WIDGET_HELP_FINANCE_FOREIGN_INCOME, WIDGET_HELP_FINANCE_INFLATED_COSTS,
		WIDGET_HELP_FINANCE_NUM_UNITS, WIDGET_HELP_FINANCE_SPECIALISTS,
		WIDGET_HELP_FINANCE_UNIT_COST,
		WIDGET_LEADERHEAD, WIDGET_LEADERHEAD_RELATIONS, WIDGET_LEADER_LINE,
		WIDGET_CONTACT_CIV, WIDGET_SCORE_BREAKDOWN, WIDGET_POWER_RATIO,
		WIDGET_GOLDEN_AGE, WIDGET_ANARCHY };
	for(int i = 0; i < sizeof(aePlayerAsData1) / sizeof(WidgetTypes); i++) {
		if(widgetDataStruct.m_eWidgetType == aePlayerAsData1[i] &&
				(widgetDataStruct.m_iData1 <= NO_PLAYER ||
				widgetDataStruct.m_iData1 >= MAX_PLAYERS)) {
			FAssertMsg(false, "Player id missing in widget data");
			return;
		}
	} // </advc.085>
	switch (widgetDataStruct.m_eWidgetType)
	{
	case WIDGET_PLOT_LIST:
		parsePlotListHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_PLOT_LIST_SHIFT:
		szBuffer.assign(gDLL->getText("TXT_KEY_MISC_CTRL_SHIFT", (GC.getDefineINT("MAX_PLOT_LIST_SIZE") - 1)));
		break;

	case WIDGET_CITY_SCROLL:
		break;

	case WIDGET_LIBERATE_CITY:
		parseLiberateCityHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_CITY_NAME:
		parseCityNameHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_UNIT_NAME:
		szBuffer.append(gDLL->getText("TXT_KEY_CHANGE_NAME"));
		break;

	case WIDGET_CREATE_GROUP:
		szBuffer.append(gDLL->getText("TXT_KEY_WIDGET_CREATE_GROUP"));
		break;

	case WIDGET_DELETE_GROUP:
		szBuffer.append(gDLL->getText("TXT_KEY_WIDGET_DELETE_GROUP"));
		break;

	case WIDGET_TRAIN:
		parseTrainHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_CONSTRUCT:
		parseConstructHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_CREATE:
		parseCreateHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_MAINTAIN:
		parseMaintainHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_HURRY:
		parseHurryHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_MENU_ICON:
		szBuffer.append(gDLL->getText("TXT_KEY_MAIN_MENU"));

	case WIDGET_CONSCRIPT:
		parseConscriptHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_ACTION:
		parseActionHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_CITIZEN:
		parseCitizenHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_FREE_CITIZEN:
		parseFreeCitizenHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_DISABLED_CITIZEN:
		parseDisabledCitizenHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_ANGRY_CITIZEN:
		parseAngryCitizenHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_CHANGE_SPECIALIST:
		parseChangeSpecialistHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_RESEARCH:
		parseResearchHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_TECH_TREE:
		parseTechTreeHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_CHANGE_PERCENT:
		parseChangePercentHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_CITY_TAB:
		{
			CvWString szTemp;
			szTemp.Format(L"%s", GC.getCityTabInfo((CityTabTypes)widgetDataStruct.m_iData1).getDescription());
			szBuffer.assign(szTemp);
		}
		break;

	case WIDGET_CONTACT_CIV:
		parseContactCivHelp(widgetDataStruct, szBuffer);
		break;
	// advc.085 (comment): Was unused in BtS; now used on the scoreboard.
	case WIDGET_SCORE_BREAKDOWN:
		parseScoreHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_ZOOM_CITY:
		szBuffer.append(gDLL->getText("TXT_KEY_ZOOM_CITY_HELP"));
		break;

	case WIDGET_END_TURN:
		szBuffer.append(gDLL->getText("TXT_KEY_WIDGET_END_TURN"));
		break;
		
	case WIDGET_LAUNCH_VICTORY:
		szBuffer.append(gDLL->getText("TXT_KEY_WIDGET_LAUNCH_VICTORY"));
		break;

	case WIDGET_AUTOMATE_CITIZENS:
		parseAutomateCitizensHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_AUTOMATE_PRODUCTION:
		parseAutomateProductionHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_EMPHASIZE:
		parseEmphasizeHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_TRADE_ITEM:
		parseTradeItem(widgetDataStruct, szBuffer);
		break;

	case WIDGET_UNIT_MODEL:
		parseUnitModelHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_FLAG:
		parseFlagHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_HELP_MAINTENANCE:
		parseMaintenanceHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_HELP_RELIGION:
		parseReligionHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_HELP_RELIGION_CITY:
		parseReligionHelpCity(widgetDataStruct, szBuffer);
		break;

	case WIDGET_HELP_CORPORATION_CITY:
		parseCorporationHelpCity(widgetDataStruct, szBuffer);
		break;

	case WIDGET_HELP_NATIONALITY:
		parseNationalityHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_HELP_DEFENSE:
		break;

	case WIDGET_HELP_HEALTH:
		parseHealthHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_HELP_HAPPINESS:
		parseHappinessHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_HELP_POPULATION:
		parsePopulationHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_HELP_PRODUCTION:
		parseProductionHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_HELP_CULTURE:
		parseCultureHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_HELP_GREAT_PEOPLE:
		parseGreatPeopleHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_HELP_GREAT_GENERAL:
		parseGreatGeneralHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_HELP_SELECTED:
		parseSelectedHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_HELP_BUILDING:
		parseBuildingHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_HELP_TRADE_ROUTE_CITY:
		parseTradeRouteCityHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_HELP_ESPIONAGE_COST:
		parseEspionageCostHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_HELP_TECH_ENTRY:
		parseTechEntryHelp(widgetDataStruct, szBuffer);
		break;
	// BULL - Trade Denial - start
	case WIDGET_PEDIA_JUMP_TO_TECH_TRADE:
		parseTechTradeEntryHelp(widgetDataStruct, szBuffer);
		break;
	// BULL - Trade Denial - end
	case WIDGET_HELP_TECH_PREPREQ:
		parseTechPrereqHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_HELP_OBSOLETE:
		parseObsoleteHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_HELP_OBSOLETE_BONUS:
		parseObsoleteBonusString(widgetDataStruct, szBuffer);
		break;

	case WIDGET_HELP_OBSOLETE_SPECIAL:
		parseObsoleteSpecialHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_HELP_MOVE_BONUS:
		parseMoveHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_HELP_FREE_UNIT:
		parseFreeUnitHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_HELP_FEATURE_PRODUCTION:
		parseFeatureProductionHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_HELP_WORKER_RATE:
		parseWorkerRateHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_HELP_TRADE_ROUTES:
		parseTradeRouteHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_HELP_HEALTH_RATE:
		parseHealthRateHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_HELP_HAPPINESS_RATE:
		parseHappinessRateHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_HELP_FREE_TECH:
		parseFreeTechHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_HELP_LOS_BONUS:
		parseLOSHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_HELP_MAP_CENTER:
		parseMapCenterHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_HELP_MAP_REVEAL:
		parseMapRevealHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_HELP_MAP_TRADE:
		parseMapTradeHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_HELP_TECH_TRADE:
		parseTechTradeHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_HELP_GOLD_TRADE:
		parseGoldTradeHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_HELP_OPEN_BORDERS:
		parseOpenBordersHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_HELP_DEFENSIVE_PACT:
		parseDefensivePactHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_HELP_PERMANENT_ALLIANCE:
		parsePermanentAllianceHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_HELP_VASSAL_STATE:
		parseVassalStateHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_HELP_BUILD_BRIDGE:
		parseBuildBridgeHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_HELP_IRRIGATION:
		parseIrrigationHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_HELP_IGNORE_IRRIGATION:
		parseIgnoreIrrigationHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_HELP_WATER_WORK:
		parseWaterWorkHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_HELP_IMPROVEMENT:
		parseBuildHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_HELP_DOMAIN_EXTRA_MOVES:
		parseDomainExtraMovesHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_HELP_ADJUST:
		parseAdjustHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_HELP_TERRAIN_TRADE:
		parseTerrainTradeHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_HELP_SPECIAL_BUILDING:
		parseSpecialBuildingHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_HELP_YIELD_CHANGE:
		parseYieldChangeHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_HELP_BONUS_REVEAL:
		parseBonusRevealHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_HELP_CIVIC_REVEAL:
		parseCivicRevealHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_HELP_PROCESS_INFO:
		parseProcessInfoHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_HELP_FOUND_RELIGION:
		parseFoundReligionHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_HELP_FOUND_CORPORATION:
		parseFoundCorporationHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_HELP_FINANCE_NUM_UNITS:
		parseFinanceNumUnits(widgetDataStruct, szBuffer);
		break;

	case WIDGET_HELP_FINANCE_UNIT_COST:
		parseFinanceUnitCost(widgetDataStruct, szBuffer);
		break;

	case WIDGET_HELP_FINANCE_AWAY_SUPPLY:
		parseFinanceAwaySupply(widgetDataStruct, szBuffer);
		break;

	case WIDGET_HELP_FINANCE_CITY_MAINT:
		parseFinanceCityMaint(widgetDataStruct, szBuffer);
		break;

	case WIDGET_HELP_FINANCE_CIVIC_UPKEEP:
		parseFinanceCivicUpkeep(widgetDataStruct, szBuffer);
		break;

	case WIDGET_HELP_FINANCE_FOREIGN_INCOME:
		parseFinanceForeignIncome(widgetDataStruct, szBuffer);
		break;

	case WIDGET_HELP_FINANCE_INFLATED_COSTS:
		parseFinanceInflatedCosts(widgetDataStruct, szBuffer);
		break;

	case WIDGET_HELP_FINANCE_GROSS_INCOME:
		parseFinanceGrossIncome(widgetDataStruct, szBuffer);
		break;

	case WIDGET_HELP_FINANCE_NET_GOLD:
		parseFinanceNetGold(widgetDataStruct, szBuffer);
		break;

	case WIDGET_HELP_FINANCE_GOLD_RESERVE:
		parseFinanceGoldReserve(widgetDataStruct, szBuffer);
		break;
	// BULL - Finance Advisor - start
	case WIDGET_HELP_FINANCE_DOMESTIC_TRADE:
		parseFinanceDomesticTrade(widgetDataStruct, szBuffer);
		break;

	case WIDGET_HELP_FINANCE_FOREIGN_TRADE:
		parseFinanceForeignTrade(widgetDataStruct, szBuffer);
		break;

	case WIDGET_HELP_FINANCE_SPECIALISTS:
		parseFinanceSpecialistGold(widgetDataStruct, szBuffer);
		break; // BULL - Finance Advisor - end

	case WIDGET_PEDIA_JUMP_TO_TECH:
		parseTechEntryHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_PEDIA_JUMP_TO_REQUIRED_TECH:
		parseTechTreePrereq(widgetDataStruct, szBuffer, false);
		break;

	case WIDGET_PEDIA_JUMP_TO_DERIVED_TECH:
		parseTechTreePrereq(widgetDataStruct, szBuffer, true);
		break;

	case WIDGET_PEDIA_JUMP_TO_UNIT:
		parseUnitHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_PEDIA_JUMP_TO_BUILDING:
		parseBuildingHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_PEDIA_BACK:
		// parsePediaBack(widgetDataStruct, szBuffer);
		break;

	case WIDGET_PEDIA_FORWARD:
		// parsePediaForward(widgetDataStruct, szBuffer);
		break;

	case WIDGET_PEDIA_JUMP_TO_BONUS:
		parseBonusHelp(widgetDataStruct, szBuffer);
		break;
	// BULL - Trade Denial - start
	case WIDGET_PEDIA_JUMP_TO_BONUS_TRADE:
		parseBonusTradeHelp(widgetDataStruct, szBuffer);
		break;
	// BULL - Trade Denial - end
	case WIDGET_PEDIA_MAIN:
		break;

	case WIDGET_PEDIA_JUMP_TO_PROMOTION:
	case WIDGET_HELP_PROMOTION:
		parsePromotionHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_CHOOSE_EVENT:
		parseEventHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_PEDIA_JUMP_TO_UNIT_COMBAT:
		parseUnitCombatHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_PEDIA_JUMP_TO_IMPROVEMENT:
		parseImprovementHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_PEDIA_JUMP_TO_CIVIC:
		parseCivicHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_PEDIA_JUMP_TO_CIV:
		parseCivilizationHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_PEDIA_JUMP_TO_LEADER:
		parseLeaderHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_PEDIA_JUMP_TO_SPECIALIST:
		if (widgetDataStruct.m_iData1 != NO_SPECIALIST && widgetDataStruct.m_iData2 != 0)
		{
			CvWString szTemp;
			szTemp.Format(L"%s", GC.getSpecialistInfo((SpecialistTypes)widgetDataStruct.m_iData1).getDescription());
			szBuffer.assign(szTemp);
		}
		break;

	case WIDGET_PEDIA_JUMP_TO_PROJECT:
		parseProjectHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_PEDIA_JUMP_TO_RELIGION:
		parseReligionHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_PEDIA_JUMP_TO_CORPORATION:
		parseCorporationHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_PEDIA_JUMP_TO_TERRAIN:
		parseTerrainHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_PEDIA_JUMP_TO_FEATURE:
		parseFeatureHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_PEDIA_DESCRIPTION:
		parseDescriptionHelp(widgetDataStruct, szBuffer, false);
		break;

	case WIDGET_CLOSE_SCREEN:
		//parseCloseScreenHelp(szBuffer);
		break;

	case WIDGET_DEAL_KILL:
		parseKillDealHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_PEDIA_DESCRIPTION_NO_HELP:
		//parseDescriptionHelp(widgetDataStruct, szBuffer, true);
		break;

	case WIDGET_MINIMAP_HIGHLIGHT:
		break;

	case WIDGET_PRODUCTION_MOD_HELP:
		parseProductionModHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_LEADERHEAD:
		parseLeaderheadHelp(widgetDataStruct, szBuffer);
		break;
	// <advc.152>
	case WIDGET_LH_GLANCE:
		//parseLeaderheadHelp(widgetDataStruct, szBuffer);
		parseLeaderheadRelationsHelp(widgetDataStruct, szBuffer); // BULL - Leaderhead Relations
		// Might as well call GAMETEXT right here
		GAMETEXT.parseWarTradesHelp(szBuffer, (PlayerTypes)widgetDataStruct.m_iData1,
				(PlayerTypes)widgetDataStruct.m_iData2);
		break; // </advc.152>
	// BULL - Leaderhead Relations - start
	case WIDGET_LEADERHEAD_RELATIONS:
		parseLeaderheadRelationsHelp(widgetDataStruct, szBuffer);
		break; // BULL - Leaderhead Relations - end

	case WIDGET_LEADER_LINE:
		parseLeaderLineHelp(widgetDataStruct, szBuffer);
		break;

	case WIDGET_COMMERCE_MOD_HELP:
		parseCommerceModHelp(widgetDataStruct, szBuffer);
		break;

	// K-Mod. Environmental advisor widgets.
	case WIDGET_HELP_POLLUTION_OFFSETS:
		parsePollutionOffsetsHelp(widgetDataStruct, szBuffer);
		break;
	case WIDGET_HELP_POLLUTION_SOURCE:
		parsePollutionHelp(widgetDataStruct, szBuffer);
		break;
	case WIDGET_HELP_SUSTAINABILITY_THRESHOLD:
		szBuffer.assign(gDLL->getText("TXT_KEY_SUSTAINABILITY_THRESHOLD_HELP"));
		break;
	case WIDGET_HELP_GW_RELATIVE_CONTRIBUTION:
		szBuffer.assign(gDLL->getText("TXT_KEY_GW_RELATIVE_CONTRIBUTION_HELP"));
		break;
	case WIDGET_HELP_GW_INDEX:
		szBuffer.assign(gDLL->getText("TXT_KEY_GW_INDEX_HELP"));
		break;
	case WIDGET_HELP_GW_UNHAPPY:
		szBuffer.assign(gDLL->getText("TXT_KEY_GW_UNHAPPY_HELP"));
		break;
	// K-Mod. Extra specialist commerce
	case WIDGET_HELP_EXTRA_SPECIALIST_COMMERCE:
		GAMETEXT.setCommerceChangeHelp(szBuffer, L"", L"", gDLL->getText("TXT_KEY_CIVIC_PER_SPECIALIST").GetCString(), GC.getTechInfo((TechTypes)(widgetDataStruct.m_iData1)).getSpecialistExtraCommerceArray(), false, false);
		break;
	// K-Mod end
	// <advc.706>
	case WIDGET_RF_CIV_CHOICE:
		GC.getGameINLINE().getRiseFall().assignCivSelectionHelp(szBuffer,
				(PlayerTypes)widgetDataStruct.m_iData1);
		break; // </advc.706>  <advc.106i>
	case WIDGET_SHOW_REPLAY:
		szBuffer.append(gDLL->getText(widgetDataStruct.m_iData1 == 0 ?
				"TXT_KEY_HOF_SHOW_REPLAY" : "TXT_KEY_HOF_WARN"));
		break; // </advc.106i>
	// BULL - Trade Hover - start
	case WIDGET_TRADE_ROUTES:
		parseTradeRoutes(widgetDataStruct, szBuffer);
		break; // BULL - Trade Hover - end
	// BULL - Food Rate Hover - start
	case WIDGET_FOOD_MOD_HELP:
		parseFoodModHelp(widgetDataStruct, szBuffer);
		break; // BULL - Food Rate Hover - end
	// <advc.085>
	case WIDGET_EXPAND_SCORES:
		break; // Handled below (not the only widget that expands the scoreboard)
	case WIDGET_POWER_RATIO:
		parsePowerRatioHelp(widgetDataStruct, szBuffer);
		break;
	case WIDGET_GOLDEN_AGE:
		parseGoldenAgeAnarchyHelp((PlayerTypes)widgetDataStruct.m_iData1,
				widgetDataStruct.m_iData2, false, szBuffer);
		break;
	case WIDGET_ANARCHY:
		parseGoldenAgeAnarchyHelp((PlayerTypes)widgetDataStruct.m_iData1,
				widgetDataStruct.m_iData2, true, szBuffer);
		break;
	}
	static WidgetTypes aeExpandTypes[] = {
		WIDGET_CONTACT_CIV, WIDGET_DEAL_KILL, WIDGET_PEDIA_JUMP_TO_TECH,
		WIDGET_EXPAND_SCORES, WIDGET_SCORE_BREAKDOWN, WIDGET_POWER_RATIO,
		WIDGET_GOLDEN_AGE, WIDGET_ANARCHY
	};
	for(int i = 0; i < sizeof(aeExpandTypes) / sizeof(WidgetTypes); i++) {
		if((widgetDataStruct.m_eWidgetType == aeExpandTypes[i] &&
				widgetDataStruct.m_iData2 == 0) ||
				// Need iData2 for sth. else; must only use these WidgetTypes on the scoreboard.
				widgetDataStruct.m_eWidgetType == WIDGET_TRADE_ROUTES ||
				widgetDataStruct.m_eWidgetType == WIDGET_POWER_RATIO)
			GET_PLAYER(GC.getGameINLINE().getActivePlayer()).setScoreboardExpanded(true);
	} // </advc.085>
}


bool CvDLLWidgetData::executeAction( CvWidgetDataStruct &widgetDataStruct )
{
	bool bHandled = false; // Right now general bHandled = false;  We can specific-case this to true later. Game will run with this = false;

	switch (widgetDataStruct.m_eWidgetType)
	{

	case WIDGET_PLOT_LIST:
		doPlotList(widgetDataStruct);
		break;

	case WIDGET_PLOT_LIST_SHIFT:
		gDLL->getInterfaceIFace()->changePlotListColumn(widgetDataStruct.m_iData1 *
			  // <advc.004n> BtS code:
			  //((GC.ctrlKey()) ? (GC.getDefineINT("MAX_PLOT_LIST_SIZE") - 1) : 1));
			  std::min(GC.getDefineINT("MAX_PLOT_LIST_SIZE"),
			  gDLL->getInterfaceIFace()->getHeadSelectedCity()->plot()->getNumUnits())
			  /* Don't really know how to determine the number of units shown
				 initially. Offset divided by 9 happens to work, at least for
				 1024x768 (offset 81, 9 units) and 1280x1024 (144, 16). */
			  - gDLL->getInterfaceIFace()->getPlotListOffset() / 9); // </advc.004n>
		break;

	case WIDGET_CITY_SCROLL:
		if ( widgetDataStruct.m_iData1 > 0 )
		{
			GC.getGameINLINE().doControl(CONTROL_NEXTCITY);
		}
		else
		{
			GC.getGameINLINE().doControl(CONTROL_PREVCITY);
		}
		break;

	case WIDGET_LIBERATE_CITY:
		doLiberateCity();
		break;

	case WIDGET_CITY_NAME:
		doRenameCity();
		break;

	case WIDGET_UNIT_NAME:
		doRenameUnit();
		break;

	case WIDGET_CREATE_GROUP:
		doCreateGroup();
		break;

	case WIDGET_DELETE_GROUP:
		doDeleteGroup();
		break;

	case WIDGET_TRAIN:
		doTrain(widgetDataStruct);
		break;

	case WIDGET_CONSTRUCT:
		doConstruct(widgetDataStruct);
		break;

	case WIDGET_CREATE:
		doCreate(widgetDataStruct);
		break;

	case WIDGET_MAINTAIN:
		doMaintain(widgetDataStruct);
		break;

	case WIDGET_HURRY:
		doHurry(widgetDataStruct);
		break;

	case WIDGET_MENU_ICON:
		doMenu();

	case WIDGET_CONSCRIPT:
		doConscript();
		break;

	case WIDGET_ACTION:
		doAction(widgetDataStruct);
		break;

	case WIDGET_CITIZEN:
		break;

	case WIDGET_FREE_CITIZEN:
		break;

	case WIDGET_DISABLED_CITIZEN:
		break;

	case WIDGET_ANGRY_CITIZEN:
		break;

	case WIDGET_CHANGE_SPECIALIST:
		doChangeSpecialist(widgetDataStruct);
		break;

	case WIDGET_RESEARCH:
	case WIDGET_TECH_TREE:
		doResearch(widgetDataStruct);
		break;

	case WIDGET_CHANGE_PERCENT:
		doChangePercent(widgetDataStruct);
		break;

	case WIDGET_CITY_TAB:
		doCityTab(widgetDataStruct);
		break;

	case WIDGET_CONTACT_CIV:
		doContactCiv(widgetDataStruct);
		break;

	case WIDGET_END_TURN:
		GC.getGameINLINE().doControl(CONTROL_FORCEENDTURN);
		break;

	case WIDGET_LAUNCH_VICTORY:
		doLaunch(widgetDataStruct);
		break;

	case WIDGET_CONVERT:
		doConvert(widgetDataStruct);
		break;

	case WIDGET_REVOLUTION:
		// handled in Python
		break;

	case WIDGET_AUTOMATE_CITIZENS:
		doAutomateCitizens();
		break;

	case WIDGET_AUTOMATE_PRODUCTION:
		doAutomateProduction();
		break;

	case WIDGET_EMPHASIZE:
		doEmphasize(widgetDataStruct);
		break;

	case WIDGET_DIPLOMACY_RESPONSE:
		// CLEANUP -- PD
		// GC.getDiplomacyScreen().handleClick(m_pWidget);
		break;

	case WIDGET_TRADE_ITEM:
		break;

	case WIDGET_UNIT_MODEL:
		doUnitModel();
		break;

	case WIDGET_FLAG:
		doFlag();
		break;

	case WIDGET_HELP_SELECTED:
		doSelected(widgetDataStruct);
		break;

	case WIDGET_PEDIA_JUMP_TO_UNIT:
		doPediaUnitJump(widgetDataStruct);
		break;

	case WIDGET_PEDIA_JUMP_TO_BUILDING:
		doPediaBuildingJump(widgetDataStruct);
		break;

	case WIDGET_PEDIA_JUMP_TO_TECH:
	case WIDGET_PEDIA_JUMP_TO_REQUIRED_TECH:
	case WIDGET_PEDIA_JUMP_TO_DERIVED_TECH:
		doPediaTechJump(widgetDataStruct);
		break;

	case WIDGET_PEDIA_BACK:
		doPediaBack();
		break;
	case WIDGET_PEDIA_FORWARD:
		doPediaForward();
		break;

	case WIDGET_PEDIA_JUMP_TO_BONUS:
		doPediaBonusJump(widgetDataStruct);
		break;

	case WIDGET_PEDIA_MAIN:
		doPediaMain(widgetDataStruct);
		break;

	case WIDGET_PEDIA_JUMP_TO_PROMOTION:
		doPediaPromotionJump(widgetDataStruct);
		break;

	case WIDGET_PEDIA_JUMP_TO_UNIT_COMBAT:
		doPediaUnitCombatJump(widgetDataStruct);
		break;

	case WIDGET_PEDIA_JUMP_TO_IMPROVEMENT:
		doPediaImprovementJump(widgetDataStruct);
		break;

	case WIDGET_PEDIA_JUMP_TO_CIVIC:
		doPediaCivicJump(widgetDataStruct);
		break;

	case WIDGET_PEDIA_JUMP_TO_CIV:
		doPediaCivilizationJump(widgetDataStruct);
		break;

	case WIDGET_PEDIA_JUMP_TO_LEADER:
		doPediaLeaderJump(widgetDataStruct);
		break;

	case WIDGET_PEDIA_JUMP_TO_SPECIALIST:
		doPediaSpecialistJump(widgetDataStruct);
		break;

	case WIDGET_PEDIA_JUMP_TO_PROJECT:
		doPediaProjectJump(widgetDataStruct);
		break;

	case WIDGET_PEDIA_JUMP_TO_RELIGION:
		doPediaReligionJump(widgetDataStruct);
		break;

	case WIDGET_PEDIA_JUMP_TO_CORPORATION:
		doPediaCorporationJump(widgetDataStruct);
		break;

	case WIDGET_PEDIA_JUMP_TO_TERRAIN:
		doPediaTerrainJump(widgetDataStruct);
		break;

	case WIDGET_PEDIA_JUMP_TO_FEATURE:
		doPediaFeatureJump(widgetDataStruct);
		break;

	case WIDGET_PEDIA_DESCRIPTION:
	case WIDGET_PEDIA_DESCRIPTION_NO_HELP:
		doPediaDescription(widgetDataStruct);
		break;

	case WIDGET_TURN_EVENT:
		doGotoTurnEvent(widgetDataStruct);
		break;

	case WIDGET_FOREIGN_ADVISOR:
		doForeignAdvisor(widgetDataStruct);
		break;

	case WIDGET_DEAL_KILL:
		doDealKill(widgetDataStruct);
		break;

	case WIDGET_MINIMAP_HIGHLIGHT:
		doRefreshMilitaryAdvisor(widgetDataStruct);
		break;

	case WIDGET_CHOOSE_EVENT:
		break;

	case WIDGET_ZOOM_CITY:
		break;
	// <advc.706>
	case WIDGET_RF_CIV_CHOICE:
		GC.getGameINLINE().getRiseFall().handleCivSelection(
				(PlayerTypes)widgetDataStruct.m_iData1);
		break;
	// </advc.706>
	case WIDGET_HELP_TECH_PREPREQ:
	case WIDGET_HELP_OBSOLETE:
	case WIDGET_HELP_OBSOLETE_BONUS:
	case WIDGET_HELP_OBSOLETE_SPECIAL:
	case WIDGET_HELP_MOVE_BONUS:
	case WIDGET_HELP_FREE_UNIT:
	case WIDGET_HELP_FEATURE_PRODUCTION:
	case WIDGET_HELP_WORKER_RATE:
	case WIDGET_HELP_TRADE_ROUTES:
	case WIDGET_HELP_HEALTH_RATE:
	case WIDGET_HELP_HAPPINESS_RATE:
	case WIDGET_HELP_FREE_TECH:
	case WIDGET_HELP_LOS_BONUS:
	case WIDGET_HELP_MAP_CENTER:
	case WIDGET_HELP_MAP_REVEAL:
	case WIDGET_HELP_MAP_TRADE:
	case WIDGET_HELP_TECH_TRADE:
	case WIDGET_HELP_GOLD_TRADE:
	case WIDGET_HELP_OPEN_BORDERS:
	case WIDGET_HELP_DEFENSIVE_PACT:
	case WIDGET_HELP_PERMANENT_ALLIANCE:
	case WIDGET_HELP_VASSAL_STATE:
	case WIDGET_HELP_BUILD_BRIDGE:
	case WIDGET_HELP_IRRIGATION:
	case WIDGET_HELP_IGNORE_IRRIGATION:
	case WIDGET_HELP_WATER_WORK:
	case WIDGET_HELP_IMPROVEMENT:
	case WIDGET_HELP_DOMAIN_EXTRA_MOVES:
	case WIDGET_HELP_ADJUST:
	case WIDGET_HELP_TERRAIN_TRADE:
	case WIDGET_HELP_SPECIAL_BUILDING:
	case WIDGET_HELP_YIELD_CHANGE:
	case WIDGET_HELP_BONUS_REVEAL:
	case WIDGET_HELP_CIVIC_REVEAL:
	case WIDGET_HELP_PROCESS_INFO:
	case WIDGET_HELP_FINANCE_NUM_UNITS:
	case WIDGET_HELP_FINANCE_UNIT_COST:
	case WIDGET_HELP_FINANCE_AWAY_SUPPLY:
	case WIDGET_HELP_FINANCE_CITY_MAINT:
	case WIDGET_HELP_FINANCE_CIVIC_UPKEEP:
	case WIDGET_HELP_FINANCE_FOREIGN_INCOME:
	case WIDGET_HELP_FINANCE_INFLATED_COSTS:
	case WIDGET_HELP_FINANCE_GROSS_INCOME:
	case WIDGET_HELP_FINANCE_NET_GOLD:
	case WIDGET_HELP_FINANCE_GOLD_RESERVE:
	case WIDGET_HELP_RELIGION_CITY:
	case WIDGET_HELP_CORPORATION_CITY:
	case WIDGET_HELP_PROMOTION:
	case WIDGET_LEADERHEAD:
	case WIDGET_LEADER_LINE:
	case WIDGET_CLOSE_SCREEN:
	case WIDGET_SCORE_BREAKDOWN:
		//	Nothing on clicked
		break;
	}

	return bHandled;
}

//	right clicking action
bool CvDLLWidgetData::executeAltAction( CvWidgetDataStruct &widgetDataStruct )
{
	CvWidgetDataStruct widgetData = widgetDataStruct;

	bool bHandled = true;
	switch (widgetDataStruct.m_eWidgetType)
	{
	case WIDGET_HELP_TECH_ENTRY:
	case WIDGET_HELP_TECH_PREPREQ:
	case WIDGET_RESEARCH:
	case WIDGET_TECH_TREE:
		doPediaTechJump(widgetDataStruct);
		break;
	// K-Mod
	case WIDGET_CHANGE_PERCENT:
		doChangePercentAlt(widgetDataStruct);
		break;
	// K-Mod end
	case WIDGET_TRAIN:
		doPediaTrainJump(widgetDataStruct);
		break;
	case WIDGET_CONSTRUCT:
		doPediaConstructJump(widgetDataStruct);
		break;
	case WIDGET_CREATE:
		doPediaProjectJump(widgetDataStruct);
		break;
	case WIDGET_PEDIA_JUMP_TO_UNIT:
	case WIDGET_HELP_FREE_UNIT:
		doPediaUnitJump(widgetDataStruct);
		break;
	case WIDGET_HELP_FOUND_RELIGION:
		widgetData.m_iData1 = widgetData.m_iData2;
		//	Intentional fallthrough...
	case WIDGET_PEDIA_JUMP_TO_RELIGION:
		doPediaReligionJump(widgetData);
		break;
	case WIDGET_HELP_FOUND_CORPORATION:
		widgetData.m_iData1 = widgetData.m_iData2;
		//	Intentional fallthrough...
	case WIDGET_PEDIA_JUMP_TO_CORPORATION:
		doPediaCorporationJump(widgetData);
		break;
	case WIDGET_PEDIA_JUMP_TO_BUILDING:
		doPediaBuildingJump(widgetDataStruct);
		break;
	case WIDGET_PEDIA_JUMP_TO_PROMOTION:
		doPediaPromotionJump(widgetDataStruct);
		break;
	case WIDGET_HELP_OBSOLETE:
		doPediaBuildingJump(widgetDataStruct);
		break;
	case WIDGET_HELP_IMPROVEMENT:
		doPediaBuildJump(widgetDataStruct);
		break;
	case WIDGET_HELP_YIELD_CHANGE:
		doPediaImprovementJump(widgetDataStruct, true);
		break;
	case WIDGET_HELP_BONUS_REVEAL:
	case WIDGET_HELP_OBSOLETE_BONUS:
		doPediaBonusJump(widgetDataStruct, true);
		break;
	case WIDGET_CITIZEN:
	case WIDGET_FREE_CITIZEN:
	case WIDGET_DISABLED_CITIZEN:
		doPediaSpecialistJump(widgetDataStruct);
		break;
	case WIDGET_PEDIA_JUMP_TO_PROJECT:
		doPediaProjectJump(widgetDataStruct);
		break;
	case WIDGET_HELP_CIVIC_REVEAL:
		widgetData.m_iData1 = widgetData.m_iData2;
		doPediaCivicJump(widgetData);
		break;
	case WIDGET_LH_GLANCE: // advc.152
	case WIDGET_LEADERHEAD:
		doContactCiv(widgetDataStruct);
		break;

	default:
		bHandled = false;
		break;
	}

	return (bHandled);
}

bool CvDLLWidgetData::isLink(const CvWidgetDataStruct &widgetDataStruct) const
{
	bool bLink = false;
	switch (widgetDataStruct.m_eWidgetType)
	{
	case WIDGET_PEDIA_JUMP_TO_TECH:
	case WIDGET_PEDIA_JUMP_TO_REQUIRED_TECH:
	case WIDGET_PEDIA_JUMP_TO_DERIVED_TECH:
	case WIDGET_PEDIA_JUMP_TO_BUILDING:
	case WIDGET_PEDIA_JUMP_TO_UNIT:
	case WIDGET_PEDIA_JUMP_TO_UNIT_COMBAT:
	case WIDGET_PEDIA_JUMP_TO_PROMOTION:
	case WIDGET_PEDIA_JUMP_TO_BONUS:
	case WIDGET_PEDIA_JUMP_TO_IMPROVEMENT:
	case WIDGET_PEDIA_JUMP_TO_CIVIC:
	case WIDGET_PEDIA_JUMP_TO_CIV:
	case WIDGET_PEDIA_JUMP_TO_LEADER:
	case WIDGET_PEDIA_JUMP_TO_SPECIALIST:
	case WIDGET_PEDIA_JUMP_TO_PROJECT:
	case WIDGET_PEDIA_JUMP_TO_RELIGION:
	case WIDGET_PEDIA_JUMP_TO_CORPORATION:
	case WIDGET_PEDIA_JUMP_TO_TERRAIN:
	case WIDGET_PEDIA_JUMP_TO_FEATURE:
	case WIDGET_PEDIA_FORWARD:
	case WIDGET_PEDIA_BACK:
	case WIDGET_PEDIA_MAIN:
	case WIDGET_TURN_EVENT:
	case WIDGET_FOREIGN_ADVISOR:
	case WIDGET_PEDIA_DESCRIPTION:
	case WIDGET_PEDIA_DESCRIPTION_NO_HELP:
	case WIDGET_MINIMAP_HIGHLIGHT:
		bLink = (widgetDataStruct.m_iData1 >= 0);
		break;
	case WIDGET_DEAL_KILL:
		{
			CvDeal* pDeal = GC.getGameINLINE().getDeal(widgetDataStruct.m_iData1);
			bLink = (NULL != pDeal && pDeal->isCancelable(GC.getGameINLINE().getActivePlayer()));
		}
		break;
	case WIDGET_CONVERT:
		bLink = (0 != widgetDataStruct.m_iData2);
		break;
	case WIDGET_GENERAL:
	case WIDGET_REVOLUTION:
		bLink = (1 == widgetDataStruct.m_iData1);
		break;
	}
	return (bLink);
}


void CvDLLWidgetData::doPlotList(CvWidgetDataStruct &widgetDataStruct)
{
	PROFILE_FUNC();

	int iUnitIndex = widgetDataStruct.m_iData1 + gDLL->getInterfaceIFace()->getPlotListColumn() - gDLL->getInterfaceIFace()->getPlotListOffset();

	CvPlot *selectionPlot = gDLL->getInterfaceIFace()->getSelectionPlot();
	CvUnit* pUnit = gDLL->getInterfaceIFace()->getInterfacePlotUnit(selectionPlot, iUnitIndex);

	if (pUnit != NULL)
	{
		if (pUnit->getOwnerINLINE() == GC.getGameINLINE().getActivePlayer())
		{
			bool bWasCityScreenUp = gDLL->getInterfaceIFace()->isCityScreenUp();

			//gDLL->getInterfaceIFace()->selectGroup(pUnit, gDLL->shiftKey(), gDLL->ctrlKey(), gDLL->altKey());
			gDLL->getInterfaceIFace()->selectGroup(pUnit, GC.shiftKey(), GC.ctrlKey() || GC.altKey(), GC.altKey()); // K-Mod

			if (bWasCityScreenUp)
			{
				gDLL->getInterfaceIFace()->lookAtSelectionPlot();
			}
		}
	}
}


void CvDLLWidgetData::doLiberateCity()
{
	GC.getGameINLINE().selectedCitiesGameNetMessage(GAMEMESSAGE_DO_TASK, TASK_LIBERATE, 0);

	gDLL->getInterfaceIFace()->clearSelectedCities();
}


void CvDLLWidgetData::doRenameCity()
{
	CvCity* pHeadSelectedCity;

	pHeadSelectedCity = gDLL->getInterfaceIFace()->getHeadSelectedCity();

	if (pHeadSelectedCity != NULL)
	{
		if (pHeadSelectedCity->getOwnerINLINE() == GC.getGameINLINE().getActivePlayer())
		{
			CvEventReporter::getInstance().cityRename(pHeadSelectedCity);
		}
	}
}


void CvDLLWidgetData::doRenameUnit()
{
	CvUnit* pHeadSelectedUnit;

	pHeadSelectedUnit = gDLL->getInterfaceIFace()->getHeadSelectedUnit();

	if (pHeadSelectedUnit != NULL)
	{
		if (pHeadSelectedUnit->getOwnerINLINE() == GC.getGameINLINE().getActivePlayer())
		{
			CvEventReporter::getInstance().unitRename(pHeadSelectedUnit);
		}
	}
}


void CvDLLWidgetData::doCreateGroup()
{
	GC.getGameINLINE().selectionListGameNetMessage(GAMEMESSAGE_JOIN_GROUP);
}


void CvDLLWidgetData::doDeleteGroup()
{
	GC.getGameINLINE().selectionListGameNetMessage(GAMEMESSAGE_JOIN_GROUP, -1, -1, -1, 0, false, true);
}


void CvDLLWidgetData::doTrain(CvWidgetDataStruct &widgetDataStruct)
{
	UnitTypes eUnit;

	eUnit = ((UnitTypes)(GC.getCivilizationInfo(GC.getGameINLINE().getActiveCivilizationType()).getCivilizationUnits(widgetDataStruct.m_iData1)));

	if (widgetDataStruct.m_iData2 != FFreeList::INVALID_INDEX)
	{
		//CvMessageControl::getInstance().sendPushOrder(widgetDataStruct.m_iData2, ORDER_TRAIN, eUnit, false, false, false);
		CvMessageControl::getInstance().sendPushOrder(widgetDataStruct.m_iData2, ORDER_TRAIN, eUnit, false, true, 0);
	}
	else
	{
		GC.getGameINLINE().selectedCitiesGameNetMessage(GAMEMESSAGE_PUSH_ORDER, ORDER_TRAIN, eUnit, -1, false, GC.altKey(), GC.shiftKey(), GC.ctrlKey());
	}

	gDLL->getInterfaceIFace()->setCityTabSelectionRow(CITYTAB_UNITS);
}


void CvDLLWidgetData::doConstruct(CvWidgetDataStruct &widgetDataStruct)
{
	BuildingTypes eBuilding;

	eBuilding = ((BuildingTypes)(GC.getCivilizationInfo(GC.getGameINLINE().getActiveCivilizationType()).getCivilizationBuildings(widgetDataStruct.m_iData1)));

	if (widgetDataStruct.m_iData2 != FFreeList::INVALID_INDEX)
	{
		//CvMessageControl::getInstance().sendPushOrder(widgetDataStruct.m_iData2, ORDER_CONSTRUCT, eBuilding, false, false, false);
		CvMessageControl::getInstance().sendPushOrder(widgetDataStruct.m_iData2, ORDER_CONSTRUCT, eBuilding, false, true, 0);
	}
	else
	{
		GC.getGameINLINE().selectedCitiesGameNetMessage(GAMEMESSAGE_PUSH_ORDER, ORDER_CONSTRUCT, eBuilding, -1, false, GC.altKey(), GC.shiftKey(), GC.ctrlKey());
	}

	if (isLimitedWonderClass((BuildingClassTypes)(widgetDataStruct.m_iData1)))
	{
		gDLL->getInterfaceIFace()->setCityTabSelectionRow(CITYTAB_WONDERS);
	}
	else
	{
		gDLL->getInterfaceIFace()->setCityTabSelectionRow(CITYTAB_BUILDINGS);
	}
}


void CvDLLWidgetData::doCreate(CvWidgetDataStruct &widgetDataStruct)
{
	if (widgetDataStruct.m_iData2 != FFreeList::INVALID_INDEX)
	{
		//CvMessageControl::getInstance().sendPushOrder(widgetDataStruct.m_iData2, ORDER_CREATE, widgetDataStruct.m_iData1, false, false, false);
		CvMessageControl::getInstance().sendPushOrder(widgetDataStruct.m_iData2, ORDER_CREATE, widgetDataStruct.m_iData1, false, true, 0);
	}
	else
	{
		GC.getGameINLINE().selectedCitiesGameNetMessage(GAMEMESSAGE_PUSH_ORDER, ORDER_CREATE, widgetDataStruct.m_iData1, -1, false, GC.altKey(), GC.shiftKey(), GC.ctrlKey());
	}

	gDLL->getInterfaceIFace()->setCityTabSelectionRow(CITYTAB_WONDERS);
}


void CvDLLWidgetData::doMaintain(CvWidgetDataStruct &widgetDataStruct)
{
	if (widgetDataStruct.m_iData2 != FFreeList::INVALID_INDEX)
	{
		//CvMessageControl::getInstance().sendPushOrder(widgetDataStruct.m_iData2, ORDER_MAINTAIN, widgetDataStruct.m_iData1, false, false, false);
		CvMessageControl::getInstance().sendPushOrder(widgetDataStruct.m_iData2, ORDER_MAINTAIN, widgetDataStruct.m_iData1, false, true, 0);
	}
	else
	{
		GC.getGameINLINE().selectedCitiesGameNetMessage(GAMEMESSAGE_PUSH_ORDER, ORDER_MAINTAIN, widgetDataStruct.m_iData1, -1, false, GC.altKey(), GC.shiftKey(), GC.ctrlKey());
	}

	gDLL->getInterfaceIFace()->setCityTabSelectionRow(CITYTAB_WONDERS);
}


void CvDLLWidgetData::doHurry(CvWidgetDataStruct &widgetDataStruct)
{
	GC.getGameINLINE().selectedCitiesGameNetMessage(GAMEMESSAGE_DO_TASK, TASK_HURRY, widgetDataStruct.m_iData1);
}


void CvDLLWidgetData::doConscript()
{
	GC.getGameINLINE().selectedCitiesGameNetMessage(GAMEMESSAGE_DO_TASK, TASK_CONSCRIPT);
}


void CvDLLWidgetData::doAction(CvWidgetDataStruct &widgetDataStruct)
{
	GC.getGameINLINE().handleAction(widgetDataStruct.m_iData1);
}


void CvDLLWidgetData::doChangeSpecialist(CvWidgetDataStruct &widgetDataStruct)
{
	GC.getGameINLINE().selectedCitiesGameNetMessage(GAMEMESSAGE_DO_TASK, TASK_CHANGE_SPECIALIST, widgetDataStruct.m_iData1, widgetDataStruct.m_iData2);
}

void CvDLLWidgetData::doResearch(CvWidgetDataStruct &widgetDataStruct)
{
	/* original bts code
	bool bShift = GC.shiftKey();
	if(!bShift) {
		if ((GetKeyState(VK_LSHIFT) & 0x8000) || (GetKeyState(VK_RSHIFT) & 0x8000))
			bShift = true;
	}*/
	bool bShift = GC.shiftKey();

	// UNOFFICIAL_PATCH, Bugfix (Free Tech Popup Fix), 12/07/09, EmperorFool: START
	if (widgetDataStruct.m_iData2 > 0)
	{
		CvPlayer& kPlayer = GET_PLAYER(GC.getGameINLINE().getActivePlayer());
		if (!kPlayer.isChoosingFreeTech())
		{
			gDLL->getInterfaceIFace()->addHumanMessage(GC.getGameINLINE().getActivePlayer(), true, GC.getEVENT_MESSAGE_TIME(), gDLL->getText("TXT_KEY_CHEATERS_NEVER_PROSPER"), NULL, MESSAGE_TYPE_MAJOR_EVENT);
			FAssertMsg(false, "doResearch called for free tech when !isChoosingFreeTech()");
			return;
		}
		else
		{
			kPlayer.changeChoosingFreeTechCount(-1);
		}
	} // UNOFFICIAL_PATCH: END

	CvMessageControl::getInstance().sendResearch(((TechTypes)widgetDataStruct.m_iData1), widgetDataStruct.m_iData2, bShift);
}


void CvDLLWidgetData::doChangePercent(CvWidgetDataStruct &widgetDataStruct)
{
	CvMessageControl::getInstance().sendPercentChange(((CommerceTypes)widgetDataStruct.m_iData1), widgetDataStruct.m_iData2);
}

// K-Mod. Right click on "change percent" buttons will set them to min / max.
void CvDLLWidgetData::doChangePercentAlt(CvWidgetDataStruct &widgetDataStruct)
{
	CvMessageControl::getInstance().sendPercentChange((CommerceTypes)widgetDataStruct.m_iData1, widgetDataStruct.m_iData2 * 100);
	// <advc.120c>
	gDLL->getInterfaceIFace()->setDirty(Espionage_Advisor_DIRTY_BIT, true);
	gDLL->getInterfaceIFace()->setDirty(PercentButtons_DIRTY_BIT, true);
	// </advc.120c>
}
// K-Mod end

void CvDLLWidgetData::doCityTab(CvWidgetDataStruct &widgetDataStruct)
{
	gDLL->getInterfaceIFace()->setCityTabSelectionRow((CityTabTypes)widgetDataStruct.m_iData1);
}

void CvDLLWidgetData::doContactCiv(CvWidgetDataStruct &widgetDataStruct)
{
	if (gDLL->isDiplomacy() || gDLL->isMPDiplomacyScreenUp())
	{
		return;
	}

	//	Do not execute this if we are trying to contact ourselves...
	if (GC.getGameINLINE().getActivePlayer() == widgetDataStruct.m_iData1)
	{
		if (!gDLL->getInterfaceIFace()->isFocusedWidget()
				// advc.085: Never minimize the scoreboard
				&& gDLL->getInterfaceIFace()->isScoresMinimized())
			gDLL->getInterfaceIFace()->toggleScoresMinimized();
		return;
	}
	// BETTER_BTS_AI_MOD, Player Interface, 01/11/09, jdog5000: START
	if (GC.shiftKey() && !GC.altKey())
	{
		if (GET_PLAYER((PlayerTypes)widgetDataStruct.m_iData1).isHuman())
		{
			if (widgetDataStruct.m_iData1 != GC.getGameINLINE().getActivePlayer())
			{
				gDLL->getInterfaceIFace()->showTurnLog((ChatTargetTypes)widgetDataStruct.m_iData1);
			}
		}
		return;
	}

	if (GC.altKey())
	{
		TeamTypes eWidgetTeam = GET_PLAYER((PlayerTypes)widgetDataStruct.m_iData1).getTeam(); // K-Mod
		if( GC.shiftKey() )
		{
			// Warning: use of this is not multiplayer compatible
			// K-Mod, since this it isn't MP compatible, I'm going to disable it for the time being.
			if (false && !GC.getGameINLINE().isGameMultiPlayer())
			{
				if (GET_TEAM(GC.getGameINLINE().getActiveTeam()).canDeclareWar(eWidgetTeam))
				{
					if( GET_TEAM(GC.getGameINLINE().getActiveTeam()).AI_getWarPlan(eWidgetTeam) == WARPLAN_PREPARING_TOTAL) 
					{
						GET_TEAM(GC.getGameINLINE().getActiveTeam()).AI_setWarPlan(eWidgetTeam, NO_WARPLAN);
					}
					else
					{
						GET_TEAM(GC.getGameINLINE().getActiveTeam()).AI_setWarPlan(eWidgetTeam, WARPLAN_PREPARING_TOTAL);
					}
					gDLL->getInterfaceIFace()->setDirty(Score_DIRTY_BIT, true);
				}
			}
			// K-Mod end
		}
		else
		{
			if (GET_TEAM(GC.getGameINLINE().getActiveTeam()).canDeclareWar(eWidgetTeam))
			{
				/* original bts code
				CvMessageControl::getInstance().sendChangeWar(GET_PLAYER((PlayerTypes)widgetDataStruct.m_iData1).getTeam(), true); */
				// K-Mod. Give us a confirmation popup...
				CvPopupInfo* pInfo = new CvPopupInfo(BUTTONPOPUP_DECLAREWARMOVE);
				if (NULL != pInfo)
				{
					pInfo->setData1(eWidgetTeam);
					pInfo->setOption1(false); // shift key
					pInfo->setFlags(1); // don't do the "move" part of the declare-war-move.
					gDLL->getInterfaceIFace()->addPopup(pInfo);
				}
				// K-Mod end
			}
			else if (GET_TEAM(eWidgetTeam).isVassal(GC.getGameINLINE().getActiveTeam()))
			{
				CvPopupInfo* pInfo = new CvPopupInfo(BUTTONPOPUP_VASSAL_DEMAND_TRIBUTE, widgetDataStruct.m_iData1);
				if (pInfo)
				{
					gDLL->getInterfaceIFace()->addPopup(pInfo, GC.getGameINLINE().getActivePlayer(), true);
				}
			}
		}
		return;
	}
	// BETTER_BTS_AI_MOD: END
	GET_PLAYER(GC.getGameINLINE().getActivePlayer()).contact((PlayerTypes)widgetDataStruct.m_iData1);
}

void CvDLLWidgetData::doConvert(CvWidgetDataStruct &widgetDataStruct)
{
	if (widgetDataStruct.m_iData2 != 0)
	{
		CvMessageControl::getInstance().sendConvert((ReligionTypes)(widgetDataStruct.m_iData1));
	}
}

void CvDLLWidgetData::doAutomateCitizens()
{
	CvCity* pHeadSelectedCity;

	pHeadSelectedCity = gDLL->getInterfaceIFace()->getHeadSelectedCity();

	if (pHeadSelectedCity != NULL)
	{
		GC.getGameINLINE().selectedCitiesGameNetMessage(GAMEMESSAGE_DO_TASK, TASK_SET_AUTOMATED_CITIZENS, -1, -1, !(pHeadSelectedCity->isCitizensAutomated()));
	}
}

void CvDLLWidgetData::doAutomateProduction()
{
	CvCity* pHeadSelectedCity;

	pHeadSelectedCity = gDLL->getInterfaceIFace()->getHeadSelectedCity();

	if (pHeadSelectedCity != NULL)
	{
		GC.getGameINLINE().selectedCitiesGameNetMessage(GAMEMESSAGE_DO_TASK, TASK_SET_AUTOMATED_PRODUCTION, -1, -1, !pHeadSelectedCity->isProductionAutomated(), GC.altKey(), GC.shiftKey(), GC.ctrlKey());
	}
}

void CvDLLWidgetData::doEmphasize(CvWidgetDataStruct &widgetDataStruct)
{
	CvCity* pHeadSelectedCity;

	pHeadSelectedCity = gDLL->getInterfaceIFace()->getHeadSelectedCity();

	if (pHeadSelectedCity != NULL)
	{
		GC.getGameINLINE().selectedCitiesGameNetMessage(GAMEMESSAGE_DO_TASK, TASK_SET_EMPHASIZE, widgetDataStruct.m_iData1, -1, !(pHeadSelectedCity->AI_isEmphasize((EmphasizeTypes)(widgetDataStruct.m_iData1))));
	}
}

void CvDLLWidgetData::doUnitModel()
{
	if (gDLL->getInterfaceIFace()->isFocused())
	{
		//	Do NOT execute if a screen is up...
		return;
	}

	gDLL->getInterfaceIFace()->lookAtSelectionPlot();
}


void CvDLLWidgetData::doFlag()
{
	GC.getGameINLINE().doControl(CONTROL_SELECTCAPITAL);
}

void CvDLLWidgetData::doSelected(CvWidgetDataStruct &widgetDataStruct)
{
	CvCity* pHeadSelectedCity;

	pHeadSelectedCity = gDLL->getInterfaceIFace()->getHeadSelectedCity();

	if (pHeadSelectedCity != NULL)
	{
		//GC.getGameINLINE().selectedCitiesGameNetMessage(GAMEMESSAGE_POP_ORDER, widgetDataStruct.m_iData1);
		GC.getGameINLINE().selectedCitiesGameNetMessage(GAMEMESSAGE_POP_ORDER, widgetDataStruct.m_iData1, -1, -1, false, GC.altKey(), GC.shiftKey(), GC.ctrlKey());
	}
}


void CvDLLWidgetData::doPediaTechJump(CvWidgetDataStruct &widgetDataStruct)
{
	CyArgsList argsList;
	argsList.add(widgetDataStruct.m_iData1);
	gDLL->getPythonIFace()->callFunction(PYScreensModule, "pediaJumpToTech", argsList.makeFunctionArgs());
}

void CvDLLWidgetData::doPediaUnitJump(CvWidgetDataStruct &widgetDataStruct)
{
	CyArgsList argsList;

	argsList.add(widgetDataStruct.m_iData1);
	gDLL->getPythonIFace()->callFunction(PYScreensModule, "pediaJumpToUnit", argsList.makeFunctionArgs());
}

void CvDLLWidgetData::doPediaBuildingJump(CvWidgetDataStruct &widgetDataStruct)
{
	CyArgsList argsList;
	argsList.add(widgetDataStruct.m_iData1);
	gDLL->getPythonIFace()->callFunction(PYScreensModule, "pediaJumpToBuilding", argsList.makeFunctionArgs());
}

void CvDLLWidgetData::doPediaProjectJump(CvWidgetDataStruct &widgetDataStruct)
{
	CyArgsList argsList;
	argsList.add(widgetDataStruct.m_iData1);
	gDLL->getPythonIFace()->callFunction(PYScreensModule, "pediaJumpToProject", argsList.makeFunctionArgs());
}

void CvDLLWidgetData::doPediaReligionJump(CvWidgetDataStruct &widgetDataStruct)
{
	CyArgsList argsList;
	argsList.add(widgetDataStruct.m_iData1);
	gDLL->getPythonIFace()->callFunction(PYScreensModule, "pediaJumpToReligion", argsList.makeFunctionArgs());
}

void CvDLLWidgetData::doPediaCorporationJump(CvWidgetDataStruct &widgetDataStruct)
{
	CyArgsList argsList;
	argsList.add(widgetDataStruct.m_iData1);
	gDLL->getPythonIFace()->callFunction(PYScreensModule, "pediaJumpToCorporation", argsList.makeFunctionArgs());
}

void CvDLLWidgetData::doPediaTerrainJump(CvWidgetDataStruct &widgetDataStruct)
{
	CyArgsList argsList;
	argsList.add(widgetDataStruct.m_iData1);
	gDLL->getPythonIFace()->callFunction(PYScreensModule, "pediaJumpToTerrain", argsList.makeFunctionArgs());
}

void CvDLLWidgetData::doPediaFeatureJump(CvWidgetDataStruct &widgetDataStruct)
{
	CyArgsList argsList;
	argsList.add(widgetDataStruct.m_iData1);
	gDLL->getPythonIFace()->callFunction(PYScreensModule, "pediaJumpToFeature", argsList.makeFunctionArgs());
}

void CvDLLWidgetData::doPediaTrainJump(CvWidgetDataStruct &widgetDataStruct)
{
	CyArgsList argsList;
	argsList.add(GC.getCivilizationInfo(GC.getGameINLINE().getActiveCivilizationType()).getCivilizationUnits(widgetDataStruct.m_iData1));
	
	gDLL->getPythonIFace()->callFunction(PYScreensModule, "pediaJumpToUnit", argsList.makeFunctionArgs());
}


void CvDLLWidgetData::doPediaConstructJump(CvWidgetDataStruct &widgetDataStruct)
{
	CyArgsList argsList;
	argsList.add(GC.getCivilizationInfo(GC.getGameINLINE().getActiveCivilizationType()).getCivilizationBuildings(widgetDataStruct.m_iData1));

	gDLL->getPythonIFace()->callFunction(PYScreensModule, "pediaJumpToBuilding", argsList.makeFunctionArgs());
}


void CvDLLWidgetData::doPediaBack()
{
	gDLL->getPythonIFace()->callFunction(PYScreensModule, "pediaBack");	
}

void CvDLLWidgetData::doPediaForward()
{
	gDLL->getPythonIFace()->callFunction(PYScreensModule, "pediaForward");	
}

void CvDLLWidgetData::doPediaBonusJump(CvWidgetDataStruct &widgetDataStruct, bool bData2)
{
	CyArgsList argsList;
	if (bData2)
	{
		argsList.add(widgetDataStruct.m_iData2);
	}
	else
	{
		argsList.add(widgetDataStruct.m_iData1);
	}
	gDLL->getPythonIFace()->callFunction(PYScreensModule, "pediaJumpToBonus", argsList.makeFunctionArgs());	
}

void CvDLLWidgetData::doPediaSpecialistJump(CvWidgetDataStruct &widgetDataStruct)
{
	CyArgsList argsList;
	argsList.add(widgetDataStruct.m_iData1);
	gDLL->getPythonIFace()->callFunction(PYScreensModule, "pediaJumpToSpecialist", argsList.makeFunctionArgs());	
}

void CvDLLWidgetData::doPediaMain(CvWidgetDataStruct &widgetDataStruct)
{
	CyArgsList argsList;
	argsList.add(widgetDataStruct.m_iData1 < 0 ? 0 : widgetDataStruct.m_iData1);
	gDLL->getPythonIFace()->callFunction(PYScreensModule, "pediaMain", argsList.makeFunctionArgs());	
}

void CvDLLWidgetData::doPediaPromotionJump(CvWidgetDataStruct &widgetDataStruct)
{
	CyArgsList argsList;
	argsList.add(widgetDataStruct.m_iData1);
	gDLL->getPythonIFace()->callFunction(PYScreensModule, "pediaJumpToPromotion", argsList.makeFunctionArgs());
}

void CvDLLWidgetData::doPediaUnitCombatJump(CvWidgetDataStruct &widgetDataStruct)
{
	CyArgsList argsList;
	argsList.add(widgetDataStruct.m_iData1);
	gDLL->getPythonIFace()->callFunction(PYScreensModule, "pediaJumpToUnitChart", argsList.makeFunctionArgs());
}

void CvDLLWidgetData::doPediaImprovementJump(CvWidgetDataStruct &widgetDataStruct, bool bData2)
{
	CyArgsList argsList;
	if (bData2)
	{
		argsList.add(widgetDataStruct.m_iData2);
	}
	else
	{
		argsList.add(widgetDataStruct.m_iData1);
	}
	gDLL->getPythonIFace()->callFunction(PYScreensModule, "pediaJumpToImprovement", argsList.makeFunctionArgs());
}

void CvDLLWidgetData::doPediaCivicJump(CvWidgetDataStruct &widgetDataStruct)
{
	CyArgsList argsList;
	argsList.add(widgetDataStruct.m_iData1);
	gDLL->getPythonIFace()->callFunction(PYScreensModule, "pediaJumpToCivic", argsList.makeFunctionArgs());
}

void CvDLLWidgetData::doPediaCivilizationJump(CvWidgetDataStruct &widgetDataStruct)
{
	CyArgsList argsList;
	argsList.add(widgetDataStruct.m_iData1);
	gDLL->getPythonIFace()->callFunction(PYScreensModule, "pediaJumpToCiv", argsList.makeFunctionArgs());
}

void CvDLLWidgetData::doPediaLeaderJump(CvWidgetDataStruct &widgetDataStruct)
{
	CyArgsList argsList;
	argsList.add(widgetDataStruct.m_iData1);
	gDLL->getPythonIFace()->callFunction(PYScreensModule, "pediaJumpToLeader", argsList.makeFunctionArgs());
}

void CvDLLWidgetData::doPediaDescription(CvWidgetDataStruct &widgetDataStruct)
{
	CyArgsList argsList;
	argsList.add(widgetDataStruct.m_iData1);
	argsList.add(widgetDataStruct.m_iData2);
	gDLL->getPythonIFace()->callFunction(PYScreensModule, "pediaShowHistorical", argsList.makeFunctionArgs());
}

void CvDLLWidgetData::doPediaBuildJump(CvWidgetDataStruct &widgetDataStruct)
{
	CyArgsList argsList;

	ImprovementTypes eImprovement = NO_IMPROVEMENT;
	BuildTypes eBuild = (BuildTypes)widgetDataStruct.m_iData2;
	if (NO_BUILD != eBuild)
	{
		eImprovement = (ImprovementTypes)GC.getBuildInfo(eBuild).getImprovement();
	}

	if (NO_IMPROVEMENT != eImprovement)
	{
		argsList.add(eImprovement);
		gDLL->getPythonIFace()->callFunction(PYScreensModule, "pediaJumpToImprovement", argsList.makeFunctionArgs());
	}
}

void CvDLLWidgetData::doGotoTurnEvent(CvWidgetDataStruct &widgetDataStruct)
{
	CvPlot* pPlot = GC.getMapINLINE().plotINLINE(widgetDataStruct.m_iData1, widgetDataStruct.m_iData2);

	if (NULL != pPlot && !gDLL->getEngineIFace()->isCameraLocked())
	{
		if (pPlot->isRevealed(GC.getGameINLINE().getActiveTeam(), false))
		{
			gDLL->getEngineIFace()->cameraLookAt(pPlot->getPoint());
		}
	}
}

void CvDLLWidgetData::doMenu( void )
{
	if (!gDLL->isGameInitializing())
	{
		CvPopupInfo* pInfo = new CvPopupInfo(BUTTONPOPUP_MAIN_MENU);
		if (NULL != pInfo)
		{
			gDLL->getInterfaceIFace()->addPopup(pInfo, NO_PLAYER, true);
		}
	}
}

void CvDLLWidgetData::doLaunch(CvWidgetDataStruct &widgetDataStruct)
{
	if (GET_TEAM(GC.getGameINLINE().getActiveTeam()).canLaunch((VictoryTypes)widgetDataStruct.m_iData1) && GC.getGameINLINE().testVictory((VictoryTypes)widgetDataStruct.m_iData1, GC.getGameINLINE().getActiveTeam()))
	{
		CvPopupInfo* pInfo = new CvPopupInfo(BUTTONPOPUP_LAUNCH, widgetDataStruct.m_iData1);
		if (NULL != pInfo)
		{
			gDLL->getInterfaceIFace()->addPopup(pInfo);
		}
	}
}

void CvDLLWidgetData::doForeignAdvisor(CvWidgetDataStruct &widgetDataStruct)
{
	CyArgsList argsList;
	argsList.add(widgetDataStruct.m_iData1);
	gDLL->getPythonIFace()->callFunction(PYScreensModule, "showForeignAdvisorScreen", argsList.makeFunctionArgs());
}

//
//	HELP PARSING FUNCTIONS
//

void CvDLLWidgetData::parsePlotListHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	PROFILE_FUNC();

	CvUnit* pUnit;

	int iUnitIndex = widgetDataStruct.m_iData1 + gDLL->getInterfaceIFace()->getPlotListColumn() - gDLL->getInterfaceIFace()->getPlotListOffset();

	CvPlot *selectionPlot = gDLL->getInterfaceIFace()->getSelectionPlot();
	pUnit = gDLL->getInterfaceIFace()->getInterfacePlotUnit(selectionPlot, iUnitIndex);

	if (pUnit != NULL)
	{
		GAMETEXT.setUnitHelp(szBuffer, pUnit,
				// <advc.069>
				false, false, false, // defaults
				pUnit->getOwnerINLINE() == GC.getGameINLINE().getActivePlayer());
				// </advc.069>
		if (pUnit->plot()->plotCount(PUF_isUnitType, pUnit->getUnitType(), -1, pUnit->getOwnerINLINE()) > 1)
		{
			szBuffer.append(NEWLINE);
			szBuffer.append(gDLL->getText("TXT_KEY_MISC_CTRL_SELECT", GC.getUnitInfo(pUnit->getUnitType()).getTextKeyWide()));
		}

		if (pUnit->plot()->plotCount(NULL, -1, -1, pUnit->getOwnerINLINE()) > 1)
		{
			szBuffer.append(NEWLINE);
			szBuffer.append(gDLL->getText("TXT_KEY_MISC_ALT_SELECT"));
		}
	}
}


void CvDLLWidgetData::parseLiberateCityHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	CvCity* pHeadSelectedCity;
	CvWString szTempBuffer;

	pHeadSelectedCity = gDLL->getInterfaceIFace()->getHeadSelectedCity();

	if (pHeadSelectedCity != NULL)
	{
		PlayerTypes ePlayer = pHeadSelectedCity->getLiberationPlayer(false);
		if (NO_PLAYER != ePlayer)
		{
			szBuffer.append(gDLL->getText("TXT_KEY_LIBERATE_CITY_HELP", pHeadSelectedCity->getNameKey(), GET_PLAYER(ePlayer).getNameKey()));
		}
	}
}

void CvDLLWidgetData::parseCityNameHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	CvCity* pHeadSelectedCity;
	CvWString szTempBuffer;

	pHeadSelectedCity = gDLL->getInterfaceIFace()->getHeadSelectedCity();

	if (pHeadSelectedCity != NULL)
	{
		szBuffer.append(pHeadSelectedCity->getName());

		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_CITY_POPULATION", pHeadSelectedCity->getRealPopulation()));

		GAMETEXT.setTimeStr(szTempBuffer, pHeadSelectedCity->getGameTurnFounded(), false);
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_CITY_FOUNDED", szTempBuffer.GetCString()));

		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_CHANGE_NAME"));
	}
}



void CvDLLWidgetData::parseTrainHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	CvCity* pHeadSelectedCity;
	UnitTypes eUnit;

	if (widgetDataStruct.m_iData2 != FFreeList::INVALID_INDEX)
	{
		pHeadSelectedCity = GET_PLAYER(GC.getGameINLINE().getActivePlayer()).getCity(widgetDataStruct.m_iData2);
	}
	else
	{
		pHeadSelectedCity = gDLL->getInterfaceIFace()->getHeadSelectedCity();
	}

	if (pHeadSelectedCity != NULL)
	{
		eUnit = (UnitTypes)GC.getCivilizationInfo(pHeadSelectedCity->getCivilizationType()).getCivilizationUnits(widgetDataStruct.m_iData1);

		GAMETEXT.setUnitHelp(szBuffer, eUnit, false, widgetDataStruct.m_bOption, false, pHeadSelectedCity);
	}
}


void CvDLLWidgetData::parseConstructHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	CvCity* pHeadSelectedCity;
	BuildingTypes eBuilding;

	if (widgetDataStruct.m_iData2 != FFreeList::INVALID_INDEX)
	{
		pHeadSelectedCity = GET_PLAYER(GC.getGameINLINE().getActivePlayer()).getCity(widgetDataStruct.m_iData2);
	}
	else
	{
		pHeadSelectedCity = gDLL->getInterfaceIFace()->getHeadSelectedCity();
	}

	if (pHeadSelectedCity != NULL)
	{
		eBuilding = (BuildingTypes)GC.getCivilizationInfo(pHeadSelectedCity->getCivilizationType()).getCivilizationBuildings(widgetDataStruct.m_iData1);

		//GAMETEXT.setBuildingHelp(szBuffer, eBuilding, false, widgetDataStruct.m_bOption, false, pHeadSelectedCity);
// BUG - Building Actual Effects - start
		GAMETEXT.setBuildingHelpActual(szBuffer, eBuilding, false, widgetDataStruct.m_bOption, false, pHeadSelectedCity);
// BUG - Building Actual Effects - end

	}
}


void CvDLLWidgetData::parseCreateHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	CvCity* pHeadSelectedCity;

	if (widgetDataStruct.m_iData2 != FFreeList::INVALID_INDEX)
	{
		pHeadSelectedCity = GET_PLAYER(GC.getGameINLINE().getActivePlayer()).getCity(widgetDataStruct.m_iData2);
	}
	else
	{
		pHeadSelectedCity = gDLL->getInterfaceIFace()->getHeadSelectedCity();
	}

	GAMETEXT.setProjectHelp(szBuffer, ((ProjectTypes)widgetDataStruct.m_iData1), false, pHeadSelectedCity);
}


void CvDLLWidgetData::parseMaintainHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	GAMETEXT.setProcessHelp(szBuffer, ((ProcessTypes)(widgetDataStruct.m_iData1)));
}


void CvDLLWidgetData::parseHurryHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	CvWString szTempBuffer;

	CvCity* pHeadSelectedCity = gDLL->getInterfaceIFace()->getHeadSelectedCity();
	if (pHeadSelectedCity == NULL)
		return;
	CvCity const& kCity = *pHeadSelectedCity;

	szBuffer.assign(gDLL->getText("TXT_KEY_MISC_HURRY_PROD", kCity.getProductionNameKey()));

	HurryTypes eHurry = (HurryTypes)widgetDataStruct.m_iData1;
	int iHurryGold = kCity.hurryGold(eHurry);

	if (iHurryGold > 0)
	{
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_MISC_HURRY_GOLD", iHurryGold));
	}

	bool bReasonGiven = false; // advc.064b: Why we can't hurry
	int iHurryPopulation = kCity.hurryPopulation(eHurry);

	if (iHurryPopulation > 0)
	{
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_MISC_HURRY_POP", iHurryPopulation));

		if (iHurryPopulation > kCity.maxHurryPopulation())
		{
			szBuffer.append(gDLL->getText("TXT_KEY_MISC_MAX_POP_HURRY", kCity.maxHurryPopulation()));
			bReasonGiven = true; // advc.064b
		}
	}

	// BUG - Hurry Overflow - start (advc.064)
	if(getBugOptionBOOL("MiscHover__HurryOverflow", true)) {
		int iOverflowProduction = 0;
		int iOverflowGold = 0;
		bool bIncludeCurrent = getBugOptionBOOL("MiscHover__HurryOverflowIncludeCurrent", false);
		if(kCity.hurryOverflow(eHurry, &iOverflowProduction, &iOverflowGold, bIncludeCurrent)) {
			if(iOverflowProduction > 0 || iOverflowGold > 0) {
				bool bFirst = true;
				CvWStringBuffer szOverflowBuffer;
				// advc: Plus signs added if !bIncludeCurrent
				if(iOverflowProduction > 0) {
					szTempBuffer.Format(L"%s%d%c", (bIncludeCurrent ? L"" : L"+"),
							iOverflowProduction, GC.getYieldInfo(YIELD_PRODUCTION).getChar());
					setListHelp(szOverflowBuffer, NULL, szTempBuffer, L", ", bFirst);
					bFirst = false;
				}
				if(iOverflowGold > 0) {
					szTempBuffer.Format(L"%s%d%c", (bIncludeCurrent ? L"" : L"+"),
							iOverflowGold, GC.getCommerceInfo(COMMERCE_GOLD).getChar());
					setListHelp(szOverflowBuffer, NULL, szTempBuffer, L", ", bFirst);
					bFirst = false;
				}
				szBuffer.append(NEWLINE);
				szBuffer.append(gDLL->getText("TXT_KEY_MISC_HURRY_OVERFLOW", szOverflowBuffer.getCString()));
			}
		}
	} // BUG - Hurry Overflow - end

	int iHurryAngerLength = kCity.hurryAngerLength(eHurry);

	if (iHurryAngerLength > 0)
	{
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_MISC_ANGER_TURNS",
				GC.getDefineINT("HURRY_POP_ANGER"),
				iHurryAngerLength + kCity.getHurryAngerTimer()));
	}

	if (!pHeadSelectedCity->isProductionUnit() && !kCity.isProductionBuilding())
	{
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_MISC_UNIT_BUILDING_HURRY"));
		bReasonGiven = true; // advc.064b
	}

	if(!kCity.canHurry(eHurry, false)) { // advc.064b
		bool bFirst = true;
		if (!GET_PLAYER(kCity.getOwnerINLINE()).canHurry(eHurry))
		{
			for (int iI = 0; iI < GC.getNumCivicInfos(); iI++)
			{
				if (GC.getCivicInfo((CivicTypes)iI).isHurry(eHurry))
				{
					szTempBuffer = NEWLINE + gDLL->getText("TXT_KEY_REQUIRES");
					setListHelp(szBuffer, szTempBuffer, GC.getCivicInfo((CivicTypes)iI).getDescription(), gDLL->getText("TXT_KEY_OR").c_str(), bFirst);
					bFirst = false;
				}
			}
			if (!bFirst)
				szBuffer.append(ENDCOLR);
		} // <advc.064b> Explain changes in CvCity::canHurry
		if(!bFirst)
			bReasonGiven = true;
		if(!bReasonGiven && kCity.getProduction() < kCity.getProductionNeeded() &&
				kCity.getCurrentProductionDifference(true, true, false, true, true)
				+ kCity.getProduction() >= kCity.getProductionNeeded()) {
			szBuffer.append(NEWLINE);
			szBuffer.append(gDLL->getText("TXT_KEY_MISC_OVERFLOW_BLOCKS_HURRY"
					/*,kCity.getProductionNameKey()*/)); // (gets too long)
			bReasonGiven = true;
		}
		if(!bReasonGiven && GC.getHurryInfo(eHurry).getGoldPerProduction() > 0 &&
				iHurryGold <= 0) {
			szBuffer.append(NEWLINE);
			szBuffer.append(gDLL->getText("TXT_KEY_MISC_PRODUCTION_BLOCKS_HURRY"));
		} // </advc.064b>
	}
}


void CvDLLWidgetData::parseConscriptHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	CvWString szTempBuffer;
	CvCity* pHeadSelectedCity = gDLL->getInterfaceIFace()->getHeadSelectedCity();
	if (pHeadSelectedCity != NULL)
	{
		if (pHeadSelectedCity->getConscriptUnit() != NO_UNIT)
		{
			CvWString szTemp;
			szTemp.Format(SETCOLR L"%s" ENDCOLR, TEXT_COLOR("COLOR_UNIT_TEXT"), GC.getUnitInfo(pHeadSelectedCity->getConscriptUnit()).getDescription());
			szBuffer.assign(szTemp);

			int iConscriptPopulation = pHeadSelectedCity->getConscriptPopulation();

			if (iConscriptPopulation > 0)
			{
				szBuffer.append(NEWLINE);
				szBuffer.append(gDLL->getText("TXT_KEY_MISC_HURRY_POP", iConscriptPopulation));
			}

			int iConscriptAngerLength = pHeadSelectedCity->flatConscriptAngerLength();

			if (iConscriptAngerLength > 0)
			{
				szBuffer.append(NEWLINE);
				szBuffer.append(gDLL->getText("TXT_KEY_MISC_ANGER_TURNS", GC.getDefineINT("CONSCRIPT_POP_ANGER"), (iConscriptAngerLength + pHeadSelectedCity->getConscriptAngerTimer())));
			}

			int iMinCityPopulation = pHeadSelectedCity->conscriptMinCityPopulation();

			if (pHeadSelectedCity->getPopulation() < iMinCityPopulation)
			{
				szBuffer.append(NEWLINE);
				szBuffer.append(gDLL->getText("TXT_KEY_MISC_MIN_CITY_POP", iMinCityPopulation));
			}

			int iMinCulturePercent = GC.getDefineINT("CONSCRIPT_MIN_CULTURE_PERCENT");

			if (pHeadSelectedCity->plot()->calculateTeamCulturePercent(pHeadSelectedCity->getTeam()) < iMinCulturePercent)
			{
				szBuffer.append(NEWLINE);
				szBuffer.append(gDLL->getText("TXT_KEY_MISC_MIN_CULTURE_PERCENT", iMinCulturePercent));
			}

			if (GET_PLAYER(pHeadSelectedCity->getOwnerINLINE()).getMaxConscript() == 0)
			{
				bool bFirst = true;

				for (int iI = 0; iI < GC.getNumCivicInfos(); iI++)
				{
					if (getWorldSizeMaxConscript((CivicTypes)iI) > 0)
					{
						szTempBuffer = NEWLINE + gDLL->getText("TXT_KEY_REQUIRES");
						setListHelp(szBuffer, szTempBuffer, GC.getCivicInfo((CivicTypes)iI).getDescription(), gDLL->getText("TXT_KEY_OR").c_str(), bFirst);
						bFirst = false;
					}
				}		

				if (!bFirst)
				{
					szBuffer.append(ENDCOLR);
				}
			}
		}
	}
}


void CvDLLWidgetData::parseActionHelp(CvWidgetDataStruct &widgetDataStruct,
		CvWStringBuffer &szBuffer)
{
	CvWString szTemp;
	CvActionInfo const& kAction = GC.getActionInfo(widgetDataStruct.m_iData1);
	szTemp.Format(SETCOLR L"%s" ENDCOLR , TEXT_COLOR("COLOR_HIGHLIGHT_TEXT"),
			kAction.getHotKeyDescription().c_str());
	szBuffer.assign(szTemp);

	CvUnit* pHeadSelectedUnit = gDLL->getInterfaceIFace()->getHeadSelectedUnit();
	if (pHeadSelectedUnit != NULL)
	{
		MissionTypes eMission = (MissionTypes)kAction.getMissionType();
		if(eMission != NO_MISSION) {
			// advc.003: Moved into subroutine
			parseActionHelp_Mission(kAction, *pHeadSelectedUnit, eMission, szBuffer);
		}
		if (kAction.getCommandType() != NO_COMMAND)
		{
			bool bAlt = GC.altKey();
			CvWString szTempBuffer;
			CLLNode<IDInfo>* pSelectedUnitNode=NULL;
			CvUnit* pSelectedUnit=NULL;
			if (kAction.getCommandType() == COMMAND_PROMOTION)
			{
				GAMETEXT.parsePromotionHelp(szBuffer,
						(PromotionTypes)kAction.getCommandData());
			}
			else if (kAction.getCommandType() == COMMAND_UPGRADE)
			{
				UnitTypes eTo = (UnitTypes)kAction.getCommandData(); // advc.003
				GAMETEXT.setBasicUnitHelp(szBuffer, eTo);

				// <advc.080>
				int iLostXP = 0;
				bool bSingleUnit = true; // </advc.080>
				int iPrice = 0;
				if (bAlt && GC.getCommandInfo((CommandTypes)kAction.getCommandType()).getAll())
				{
					CvPlayer const& kHeadOwner = GET_PLAYER(pHeadSelectedUnit->getOwnerINLINE());
					UnitTypes eFrom = pHeadSelectedUnit->getUnitType();
					iPrice = kHeadOwner.upgradeAllPrice(eTo, eFrom);
					// <advc.080>
					iLostXP = -kHeadOwner.upgradeAllXPChange(eTo, eFrom);
					bSingleUnit = false; // </advc.080>
				}
				else
				{
					pSelectedUnitNode = gDLL->getInterfaceIFace()->headSelectionListNode();
					while (pSelectedUnitNode != NULL)
					{
						pSelectedUnit = ::getUnit(pSelectedUnitNode->m_data);
						if (pSelectedUnit->canUpgrade(eTo, true))
						{
							iPrice += pSelectedUnit->upgradePrice(eTo);
							// advc.080:
							iLostXP -= pSelectedUnit->upgradeXPChange(eTo);
						}
						pSelectedUnitNode = gDLL->getInterfaceIFace()->nextSelectionListNode(pSelectedUnitNode);
						// <advc.080>
						if(pSelectedUnitNode != NULL)
							bSingleUnit = false; // </advc.080>
					}
				} // <advc.080>
				if(iLostXP > 0) {
					szBuffer.append(NEWLINE);
					if(bSingleUnit)
						szBuffer.append(gDLL->getText("TXT_KEY_MISC_LOST_XP", iLostXP));
					else szBuffer.append(gDLL->getText("TXT_KEY_MISC_LOST_XP_TOTAL", iLostXP));
				} // </advc.080>
				szTempBuffer.Format(L"%s%d %c", NEWLINE, iPrice, GC.getCommerceInfo(COMMERCE_GOLD).getChar());
				szBuffer.append(szTempBuffer);
			}
			else if (kAction.getCommandType() == COMMAND_GIFT)
			{
				PlayerTypes eGiftPlayer = pHeadSelectedUnit->plot()->getOwnerINLINE();

				if (eGiftPlayer != NO_PLAYER)
				{
					szBuffer.append(NEWLINE);
					szBuffer.append(gDLL->getText("TXT_KEY_ACTION_GOES_TO_CIV"));

					szTempBuffer.Format(SETCOLR L"%s" ENDCOLR,
							PLAYER_TEXT_COLOR(GET_PLAYER(eGiftPlayer)),
							GET_PLAYER(eGiftPlayer).getCivilizationShortDescription());
					szBuffer.append(szTempBuffer);

					pSelectedUnitNode = gDLL->getInterfaceIFace()->headSelectionListNode();

					while (pSelectedUnitNode != NULL)
					{
						pSelectedUnit = ::getUnit(pSelectedUnitNode->m_data);

						if (!(GET_PLAYER(eGiftPlayer).AI_acceptUnit(pSelectedUnit)))
						{
							szBuffer.append(NEWLINE);
							szBuffer.append(gDLL->getText("TXT_KEY_REFUSE_GIFT",
									GET_PLAYER(eGiftPlayer).getNameKey()));
							break;
						}

						pSelectedUnitNode = gDLL->getInterfaceIFace()->nextSelectionListNode(pSelectedUnitNode);
					}
				}
			}
			CvCommandInfo const& kCommand = GC.getCommandInfo((CommandTypes)kAction.getCommandType());
			if (kCommand.getAll())
			{
				szBuffer.append(gDLL->getText("TXT_KEY_ACTION_ALL_UNITS"));
			}

			if (!CvWString(kCommand.getHelp()).empty())
			{
				szBuffer.append(NEWLINE);
				// <advc.004g>
				if(kAction.getCommandType() == COMMAND_LOAD && pHeadSelectedUnit != NULL &&
						pHeadSelectedUnit->isCargo())
					szBuffer.append(gDLL->getText("TXT_KEY_COMMAND_TRANSFER_HELP"));
				else // </advc.004g>
					szBuffer.append(kCommand.getHelp());
			}
			// <advc.004b>
			if(kAction.getCommandType() == COMMAND_DELETE) {
				CvPlayer const& kActivePl = GET_PLAYER(GC.getGameINLINE().getActivePlayer());
				szBuffer.append(L" (");
				int iCurrentUnitExpenses = kActivePl.calculateUnitSupply() +
						kActivePl.calculateUnitCost();
				// Needed in order to account for inflation
				int iOtherExpenses = kActivePl.getTotalMaintenance() +
						kActivePl.getCivicUpkeep();
				int iInflationPercent = kActivePl.calculateInflationRate();
				int iCurrentExpenses = ((iOtherExpenses + iCurrentUnitExpenses) *
						(iInflationPercent + 100)) / 100;
				FAssert(iCurrentExpenses == kActivePl.calculateInflatedCosts());
				int iExtraCost = 0;
				int iUnits = 0;
				pSelectedUnitNode = gDLL->getInterfaceIFace()->headSelectionListNode();
				while(pSelectedUnitNode != NULL) {
					CvUnit const& u = *::getUnit(pSelectedUnitNode->m_data);
					pSelectedUnitNode = gDLL->getInterfaceIFace()->nextSelectionListNode(pSelectedUnitNode);
					iExtraCost += u.getUnitInfo().getExtraCost();
					iUnits--;
					/*  No danger of double counting b/c it's not possible to select
						a transport and its cargo at the same time */
					std::vector<CvUnit*> apCargo;
					u.getCargoUnits(apCargo);
					for(size_t i = 0; i < apCargo.size(); i++) {
						iExtraCost += apCargo[i]->getUnitInfo().getExtraCost();
						iUnits--;
					}
				}
				int iProjectedSupply = 0;
				bool bSupply = (pHeadSelectedUnit->plot()->getTeam() != kActivePl.getTeam());
				iProjectedSupply = kActivePl.calculateUnitSupply(bSupply ? iUnits : 0);
				int iProjectedUnitCost = kActivePl.calculateUnitCost(0, iUnits);
				int iProjectedExpenses = iProjectedSupply + iProjectedUnitCost +
						iOtherExpenses - iExtraCost;
				iProjectedExpenses = (iProjectedExpenses *
						(iInflationPercent + 100)) / 100;
				FAssert(iExtraCost >= 0 && iProjectedExpenses >= 0);
				int iGold = iCurrentExpenses - iProjectedExpenses;
				if(iGold > 0) {
					szBuffer.append(gDLL->getText("TXT_KEY_COMMAND_DELETE_DECREASE",
							iGold));
				}
				else {
					FAssert(iGold == 0);
					int iMaxTries = 5;
					int iDeltaUnits = 0;
					int iOldProj = iProjectedExpenses;
					for(int i = 1; i <= iMaxTries; i++) {
						/*  Assume that the additional units are inside borders:
							only recompute UnitCost. */
						iProjectedExpenses = iProjectedSupply - iExtraCost +
								iOtherExpenses;
						iProjectedExpenses += kActivePl.calculateUnitCost(0, iUnits - i);
						iProjectedExpenses = (iProjectedExpenses *
								(iInflationPercent + 100)) / 100;
						if(iProjectedExpenses < iOldProj) {
							iDeltaUnits = i;
							break;
						}
					}
					if(iDeltaUnits > 0) {
						if(iDeltaUnits == 1)
							szBuffer.append(gDLL->getText("TXT_KEY_COMMAND_DELETE_ONE_MORE"));
						else szBuffer.append(gDLL->getText("TXT_KEY_COMMAND_DELETE_MORE",
								iDeltaUnits));
					}
					else if(bSupply)
						szBuffer.append(gDLL->getText("TXT_KEY_COMMAND_DELETE_NO_DECREASE"));
					else szBuffer.append(gDLL->getText("TXT_KEY_COMMAND_DELETE_NO_DECREASE_UNIT_COST"));
				}
				szBuffer.append(L")");
			} // </advc.004b>
		}

		if (kAction.getAutomateType() != NO_AUTOMATE)
		{
			CvAutomateInfo const& kAutomate = GC.getAutomateInfo((ControlTypes)
					kAction.getAutomateType());
			if (!CvWString(kAutomate.getHelp()).empty())
			{
				szBuffer.append(NEWLINE);
				szBuffer.append(kAutomate.getHelp());
			}
		}
	}

	if (kAction.getControlType() != NO_CONTROL)
	{
		CvControlInfo const& kControl = GC.getControlInfo((ControlTypes)
				kAction.getControlType());
		if (!CvWString(kControl.getHelp()).empty())
		{
			szBuffer.append(NEWLINE);
			szBuffer.append(kControl.getHelp());
		}
	}

	if (kAction.getInterfaceModeType() != NO_INTERFACEMODE)
	{
		CvInterfaceModeInfo const& kIMode = GC.getInterfaceModeInfo((InterfaceModeTypes)
				kAction.getInterfaceModeType());
		if (!CvWString(kIMode.getHelp()).empty()) {
			szBuffer.append(NEWLINE);
			szBuffer.append(kIMode.getHelp());
		}
	}
}


// advc.003: Cut from parseActionHelp, refactored.
void CvDLLWidgetData::parseActionHelp_Mission(CvActionInfo const& kAction,
		CvUnit const& kUnit, MissionTypes eMission, CvWStringBuffer& szBuffer) {

	CLLNode<IDInfo>* pSelectedUnitNode=NULL;
	CvUnit* pSelectedUnit=NULL;

	CvGame const& g = GC.getGameINLINE();
	CvPlayer const& kUnitOwner = GET_PLAYER(kUnit.getOwnerINLINE());
	CvTeam const& kUnitTeam = GET_TEAM(kUnit.getTeam());
	bool bShift = GC.shiftKey();
	CvWString szTempBuffer;
	CvWString szFirstBuffer;

	CvPlot const& kMissionPlot = (
			(bShift && gDLL->getInterfaceIFace()->mirrorsSelectionGroup()) ?
			*kUnit.getGroup()->lastMissionPlot() :
			*kUnit.plot());
	CvCity* pMissionCity = kMissionPlot.getPlotCity();

	switch(eMission) { // advc.003: was if/else
	case MISSION_SENTRY_HEAL: // advc.004l
	case MISSION_HEAL:
	{
		int iTurns = 0;
		pSelectedUnitNode = gDLL->getInterfaceIFace()->headSelectionListNode();
		while (pSelectedUnitNode != NULL)
		{
			pSelectedUnit = ::getUnit(pSelectedUnitNode->m_data);
			iTurns = std::max(iTurns, pSelectedUnit->healTurns(&kMissionPlot));

			pSelectedUnitNode = gDLL->getInterfaceIFace()->nextSelectionListNode(pSelectedUnitNode);
		}
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_MISC_TURN_OR_TURNS", iTurns));
		break;
	}
	case MISSION_PILLAGE:
	{
		if (kMissionPlot.getImprovementType() != NO_IMPROVEMENT)
		{
			szBuffer.append(NEWLINE);
			szBuffer.append(gDLL->getText("TXT_KEY_ACTION_DESTROY_IMP",
					GC.getImprovementInfo(kMissionPlot.getImprovementType()).
					getTextKeyWide()));
		}
		else if (kMissionPlot.getRouteType() != NO_ROUTE)
		{
			szBuffer.append(NEWLINE);
			szBuffer.append(gDLL->getText("TXT_KEY_ACTION_DESTROY_IMP",
					GC.getRouteInfo(kMissionPlot.getRouteType()).getTextKeyWide()));
		}
		break;
	}
	case MISSION_PLUNDER:
	{
		//if (kMissionPlot.getTeam() == kUnitTeam.getID())
		if(!kUnit.canPlunder(&kMissionPlot)) // advc.033
		{
			szBuffer.append(NEWLINE);
			szBuffer.append(gDLL->getText("TXT_KEY_ACTION_PLUNDER_IN_BORDERS"));
		}
		break;
	}
	case MISSION_SABOTAGE:
	{
		pSelectedUnitNode = gDLL->getInterfaceIFace()->headSelectionListNode();
		while (pSelectedUnitNode != NULL)
		{
			pSelectedUnit = ::getUnit(pSelectedUnitNode->m_data);
			// XXX if queuing up this action, use the current plot along the goto...
			if (pSelectedUnit->canSabotage(&kMissionPlot, true))
			{
				int iPrice = pSelectedUnit->sabotageCost(&kMissionPlot);
				if (iPrice > 0)
				{
					szTempBuffer.Format(L"%d %c", iPrice,
							GC.getCommerceInfo(COMMERCE_GOLD).getChar());
					szBuffer.append(NEWLINE);
					szBuffer.append(szTempBuffer);
				}
				int iLow = pSelectedUnit->sabotageProb(&kMissionPlot, PROBABILITY_LOW);
				int iHigh = pSelectedUnit->sabotageProb(&kMissionPlot, PROBABILITY_HIGH);
				if (iLow == iHigh)
				{
					szBuffer.append(NEWLINE);
					szBuffer.append(gDLL->getText("TXT_KEY_ACTION_PROBABILITY", iHigh));
				}
				else
				{
					szBuffer.append(NEWLINE);
					szBuffer.append(gDLL->getText("TXT_KEY_ACTION_PROBABILITY_RANGE",
							iLow, iHigh));
				}
				break;
			}
			pSelectedUnitNode = gDLL->getInterfaceIFace()->nextSelectionListNode(pSelectedUnitNode);
		}
		break;
	}
	case MISSION_DESTROY:
	{
		pSelectedUnitNode = gDLL->getInterfaceIFace()->headSelectionListNode();
		while (pSelectedUnitNode != NULL)
		{
			pSelectedUnit = ::getUnit(pSelectedUnitNode->m_data);
			// XXX if queuing up this action, use the current plot along the goto...
			if (pSelectedUnit->canDestroy(&kMissionPlot, true))
			{
				int iPrice = pSelectedUnit->destroyCost(&kMissionPlot);
				if (iPrice > 0)
				{
					szTempBuffer.Format(L"%d %c", iPrice,
							GC.getCommerceInfo(COMMERCE_GOLD).getChar());
					szBuffer.append(NEWLINE);
					szBuffer.append(szTempBuffer);
				}
				int iLow = pSelectedUnit->destroyProb(&kMissionPlot, PROBABILITY_LOW);
				int iHigh = pSelectedUnit->destroyProb(&kMissionPlot, PROBABILITY_HIGH);
				if (iLow == iHigh)
				{
					szBuffer.append(NEWLINE);
					szBuffer.append(gDLL->getText("TXT_KEY_ACTION_PROBABILITY", iHigh));
				}
				else
				{
					szBuffer.append(NEWLINE);
					szBuffer.append(gDLL->getText("TXT_KEY_ACTION_PROBABILITY_RANGE",
							iLow, iHigh));
				}
				break;
			}
			pSelectedUnitNode = gDLL->getInterfaceIFace()->nextSelectionListNode(pSelectedUnitNode);
		}
		break;
	}
	case MISSION_STEAL_PLANS:
	{
		pSelectedUnitNode = gDLL->getInterfaceIFace()->headSelectionListNode();
		while (pSelectedUnitNode != NULL)
		{
			pSelectedUnit = ::getUnit(pSelectedUnitNode->m_data);
			// XXX if queuing up this action, use the current plot along the goto...
			if (pSelectedUnit->canStealPlans(&kMissionPlot, true))
			{
				int iPrice = pSelectedUnit->stealPlansCost(&kMissionPlot);
				if (iPrice > 0)
				{
					szTempBuffer.Format(L"%d %c", iPrice,
							GC.getCommerceInfo(COMMERCE_GOLD).getChar());
					szBuffer.append(NEWLINE);
					szBuffer.append(szTempBuffer);
				}
				int iLow = pSelectedUnit->stealPlansProb(&kMissionPlot, PROBABILITY_LOW);
				int iHigh = pSelectedUnit->stealPlansProb(&kMissionPlot, PROBABILITY_HIGH);
				if (iLow == iHigh)
				{
					szBuffer.append(NEWLINE);
					szBuffer.append(gDLL->getText("TXT_KEY_ACTION_PROBABILITY", iHigh));
				}
				else
				{
					szBuffer.append(NEWLINE);
					szBuffer.append(gDLL->getText("TXT_KEY_ACTION_PROBABILITY_RANGE",
							iLow, iHigh));
				}
				break;
			}
			pSelectedUnitNode = gDLL->getInterfaceIFace()->nextSelectionListNode(pSelectedUnitNode);
		}
		break;
	}
	case MISSION_FOUND:
	{
		if (!kUnitOwner.canFound(kMissionPlot.getX_INLINE(), kMissionPlot.getY_INLINE()))
		{
			bool bValid = true;
			int iRange = GC.getMIN_CITY_RANGE();
			for (int iDX = -(iRange); iDX <= iRange; iDX++)
			{
				for (int iDY = -(iRange); iDY <= iRange; iDY++)
				{
					CvPlot* pLoopPlot = plotXY(kMissionPlot.getX_INLINE(),
							kMissionPlot.getY_INLINE(), iDX, iDY);
					if (pLoopPlot != NULL)
					{
						if (pLoopPlot->isCity())
							bValid = false;
					}
				}
			}
			if (!bValid)
			{
				szBuffer.append(NEWLINE);
				szBuffer.append(gDLL->getText("TXT_KEY_ACTION_CANNOT_FOUND",
						GC.getMIN_CITY_RANGE()));
			}
		}
		// <advc.004b> Show the projected increase in city maintenance
		else {
			// No projection for the initial city
			if(kUnitOwner.getNumCities() > 0)
				szBuffer.append(getFoundCostText(kMissionPlot, kUnitOwner.getID()));
			szBuffer.append(getHomePlotYieldText(kMissionPlot, kUnitOwner.getID()));
			szBuffer.append(getNetFeatureHealthText(kMissionPlot, kUnitOwner.getID()));
			// To set the info apart from TXT_KEY_MISSION_BUILD_CITY_HELP
			szBuffer.append(NEWLINE);
		} // </advc.004b>
		break;
	}
	case MISSION_SPREAD:
	{
		ReligionTypes eReligion = (ReligionTypes)kAction.getMissionData();
		if(pMissionCity == NULL)
			break;
		if (pMissionCity->getTeam() != kUnitTeam.getID()) // XXX still true???
		{
			if (GET_PLAYER(pMissionCity->getOwnerINLINE()).isNoNonStateReligionSpread())
			{
				if (eReligion != GET_PLAYER(pMissionCity->getOwnerINLINE()).
						getStateReligion())
				{
					szBuffer.append(NEWLINE);
					szBuffer.append(gDLL->getText("TXT_KEY_ACTION_CANNOT_SPREAD_NON_STATE_RELIGION"));
				}
			}
		}

		szBuffer.append(NEWLINE);
		GAMETEXT.setReligionHelpCity(szBuffer, eReligion, pMissionCity, false, true);
		break;
	}
	case MISSION_SPREAD_CORPORATION:
	{
		CorporationTypes eCorporation = (CorporationTypes)kAction.getMissionData();
		if(pMissionCity == NULL)
			break;
		szBuffer.append(NEWLINE);
		GAMETEXT.setCorporationHelpCity(szBuffer, eCorporation, pMissionCity, false, true);
		for (int i = 0; i < GC.getNumCorporationInfos(); i++)
		{
			CorporationTypes eLoopCorp = (CorporationTypes)i;
			if (eCorporation != eLoopCorp)
			{
				if (pMissionCity->isHasCorporation(eLoopCorp))
				{
					if (g.isCompetingCorporation(eCorporation, eLoopCorp))
					{
						szBuffer.append(NEWLINE);
						szBuffer.append(gDLL->getText("TXT_KEY_ACTION_WILL_ELIMINATE_CORPORATION",
								GC.getCorporationInfo(eLoopCorp).getTextKeyWide()));
					}
				}
			}
		}
		CvPlayer const& kMissionCityOwner = GET_PLAYER(pMissionCity->getOwnerINLINE());
		szTempBuffer.Format(L"%s%d %c", NEWLINE, kUnit.spreadCorporationCost(
				eCorporation, pMissionCity),
				GC.getCommerceInfo(COMMERCE_GOLD).getChar());
		szBuffer.append(szTempBuffer);
		if (!kUnit.canSpreadCorporation(&kMissionPlot, eCorporation))
		{
			if (!kMissionCityOwner.isActiveCorporation(eCorporation))
			{
				szBuffer.append(NEWLINE);
				szBuffer.append(gDLL->getText("TXT_KEY_ACTION_CORPORATION_NOT_ACTIVE",
						GC.getCorporationInfo(eCorporation).getTextKeyWide(),
						kMissionCityOwner.getCivilizationAdjective()));
			}
			CorporationTypes eCompetition = NO_CORPORATION;
			for (int i = 0; i < GC.getNumCorporationInfos(); i++)
			{
				CorporationTypes eLoopCorp = (CorporationTypes)i;
				if (pMissionCity->isHeadquarters(eLoopCorp))
				{
					if (g.isCompetingCorporation(eLoopCorp, eCorporation))
					{
						eCompetition = eLoopCorp;
						break;
					}
				}
			}
			if (NO_CORPORATION != eCompetition)
			{
				szBuffer.append(NEWLINE);
				szBuffer.append(gDLL->getText("TXT_KEY_ACTION_CORPORATION_COMPETING_HEADQUARTERS",
						GC.getCorporationInfo(eCorporation).getTextKeyWide(),
						GC.getCorporationInfo(eCompetition).getTextKeyWide()));
			}
			CvWStringBuffer szBonusList;
			bool bValid = false;
			bool bFirst = true;
			for (int i = 0; i < GC.getNUM_CORPORATION_PREREQ_BONUSES(); ++i)
			{
				BonusTypes eBonus = (BonusTypes)GC.getCorporationInfo(eCorporation).
						getPrereqBonus(i);
				if (NO_BONUS == eBonus)
					continue;
					if (!bFirst)
						szBonusList.append(L", ");
					else bFirst = false;
					szBonusList.append(GC.getBonusInfo(eBonus).getDescription());
					if (pMissionCity->hasBonus(eBonus))
					{
						bValid = true;
						break;
					}
			}
			if (!bValid)
			{
				szBuffer.append(NEWLINE);
				szBuffer.append(gDLL->getText("TXT_KEY_ACTION_CORPORATION_NO_RESOURCES",
						pMissionCity->getNameKey(), szBonusList.getCString()));
			}
		}
		break;
	}
	case MISSION_JOIN:
	{
		GAMETEXT.parseSpecialistHelp(szBuffer, (SpecialistTypes)
				kAction.getMissionData(), pMissionCity, true);
		break;
	}
	case MISSION_CONSTRUCT:
	{
		BuildingTypes eBuilding = (BuildingTypes)kAction.getMissionData();
		if(pMissionCity == NULL)
			break;
		if (!kUnit.getUnitInfo().getForceBuildings(eBuilding) &&
				!pMissionCity->canConstruct(eBuilding, false, false, true))
		{
			if (!g.isBuildingClassMaxedOut((BuildingClassTypes)
					(GC.getBuildingInfo(eBuilding).getBuildingClassType())))
			{
				GAMETEXT.buildBuildingRequiresString(szBuffer,(BuildingTypes)kAction.
						getMissionData(), false, false, pMissionCity);
			}
		}
		else
		{
			szBuffer.append(NEWLINE);
			//GAMETEXT.setBuildingHelp(szBuffer, (BuildingTypes)kAction.getMissionData(), false, false, false, pMissionCity);
			// BUG - Building Actual Effects - start
			GAMETEXT.setBuildingHelpActual(szBuffer, (BuildingTypes)
					kAction.getMissionData(), false, false, false, pMissionCity);
			// BUG - Building Actual Effects - end
		}
		break;
	}
	case MISSION_DISCOVER:
	{
		pSelectedUnitNode = gDLL->getInterfaceIFace()->headSelectionListNode();
		while (pSelectedUnitNode != NULL)
		{
			pSelectedUnit = ::getUnit(pSelectedUnitNode->m_data);
			if (pSelectedUnit->canDiscover(&kMissionPlot))
			{
				TechTypes eTech = pSelectedUnit->getDiscoveryTech();
				int iResearchLeft = GET_TEAM(pSelectedUnit->getTeam()).getResearchLeft(eTech);
				if (pSelectedUnit->getDiscoverResearch(eTech) >= iResearchLeft)
				{
					szBuffer.append(NEWLINE);
					szTempBuffer.Format(SETCOLR L"%s" ENDCOLR, TEXT_COLOR("COLOR_TECH_TEXT"),
							GC.getTechInfo(eTech).getDescription());
					szBuffer.append(szTempBuffer);
					// <advc.004a>
					/*  Probably not a good idea after all. Players might
						not get that this is the amount of research left;
						they could assume that they're only getting
						(partial) progress toward eTech. */
					/*if(iResearchLeft > 0) {
						szTempBuffer.Format(L" (%d%c)", iResearchLeft,
								GC.getCommerceInfo(COMMERCE_RESEARCH).
								getChar());
						szBuffer.append(szTempBuffer);
					}*/ // </advc.004a>
				}
				else
				{
					szBuffer.append(NEWLINE);
					szBuffer.append(gDLL->getText("TXT_KEY_ACTION_EXTRA_RESEARCH",
							pSelectedUnit->getDiscoverResearch(eTech),
							GC.getTechInfo(eTech).getTextKeyWide()));
				}
				break;
			}

			pSelectedUnitNode = gDLL->getInterfaceIFace()->nextSelectionListNode(pSelectedUnitNode);
		}
		break;
	}
	case MISSION_HURRY:
	{
		if(pMissionCity == NULL)
			break;
		if (!pMissionCity->isProductionBuilding())
		{
			szBuffer.append(NEWLINE);
			szBuffer.append(gDLL->getText("TXT_KEY_ACTION_BUILDING_HURRY"));
			break;
		}
		pSelectedUnitNode = gDLL->getInterfaceIFace()->headSelectionListNode();
		while (pSelectedUnitNode != NULL)
		{
			pSelectedUnit = ::getUnit(pSelectedUnitNode->m_data);
			if (pSelectedUnit->canHurry(&kMissionPlot, true))
			{
				const wchar* pcKey = NULL;
				if (NO_PROJECT != pMissionCity->getProductionProject())
					pcKey = GC.getProjectInfo(pMissionCity->getProductionProject()).getTextKeyWide();
				else if (NO_BUILDING != pMissionCity->getProductionBuilding())
					pcKey = GC.getBuildingInfo(pMissionCity->getProductionBuilding()).getTextKeyWide();
				else if (NO_UNIT != pMissionCity->getProductionUnit())
					pcKey = GC.getUnitInfo(pMissionCity->getProductionUnit()).getTextKeyWide();
				if (NULL != pcKey && pSelectedUnit->getHurryProduction(&kMissionPlot) >=
						pMissionCity->productionLeft())
				{
					szBuffer.append(NEWLINE);
					szBuffer.append(gDLL->getText("TXT_KEY_ACTION_FINISH_CONSTRUCTION",
							pcKey));
				}
				else
				{
					szBuffer.append(NEWLINE);
					szBuffer.append(gDLL->getText("TXT_KEY_ACTION_EXTRA_CONSTRUCTION",
							pSelectedUnit->getHurryProduction(&kMissionPlot), pcKey));
				}
				break;
			}
			pSelectedUnitNode = gDLL->getInterfaceIFace()->nextSelectionListNode(pSelectedUnitNode);
		}
		break;
	}
	case MISSION_TRADE:
	{
		if(pMissionCity == NULL)
			break;
		if (pMissionCity->getOwnerINLINE() == kUnitOwner.getID())
		{
			szBuffer.append(NEWLINE);
			szBuffer.append(gDLL->getText("TXT_KEY_ACTION_TRADE_MISSION_FOREIGN"));
			break;
		}
		pSelectedUnitNode = gDLL->getInterfaceIFace()->headSelectionListNode();
		while (pSelectedUnitNode != NULL)
		{
			pSelectedUnit = ::getUnit(pSelectedUnitNode->m_data);
			if (pSelectedUnit->canTrade(&kMissionPlot, true))
			{
				szTempBuffer.Format(L"%s+%d%c", NEWLINE,
						pSelectedUnit->getTradeGold(&kMissionPlot),
						GC.getCommerceInfo(COMMERCE_GOLD).getChar());
				szBuffer.append(szTempBuffer);
				break;
			}
			pSelectedUnitNode = gDLL->getInterfaceIFace()->nextSelectionListNode(pSelectedUnitNode);
		}
		break;
	}
	case MISSION_GREAT_WORK:
	{
		pSelectedUnitNode = gDLL->getInterfaceIFace()->headSelectionListNode();
		while (pSelectedUnitNode != NULL)
		{
			pSelectedUnit = ::getUnit(pSelectedUnitNode->m_data);
			if (pSelectedUnit->canGreatWork(&kMissionPlot))
			{
				szTempBuffer.Format(L"%s+%d%c", NEWLINE,
						pSelectedUnit->getGreatWorkCulture(&kMissionPlot),
						GC.getCommerceInfo(COMMERCE_CULTURE).getChar());
				szBuffer.append(szTempBuffer);
				break;
			}
			pSelectedUnitNode = gDLL->getInterfaceIFace()->nextSelectionListNode(
					pSelectedUnitNode);
		}
		break;
	}
	case MISSION_INFILTRATE:
	{
		if(pMissionCity == NULL)
			break;
		if (pMissionCity->getOwnerINLINE() == kUnitOwner.getID())
		{
			szBuffer.append(NEWLINE);
			szBuffer.append(gDLL->getText("TXT_KEY_ACTION_INFILTRATE_MISSION_FOREIGN"));
			break;
		}
		pSelectedUnitNode = gDLL->getInterfaceIFace()->headSelectionListNode();
		while (pSelectedUnitNode != NULL)
		{
			pSelectedUnit = ::getUnit(pSelectedUnitNode->m_data);
			if (pSelectedUnit->canEspionage(&kMissionPlot))
			{
				szTempBuffer.Format(L"%s+%d%c", NEWLINE,
						pSelectedUnit->getEspionagePoints(&kMissionPlot),
						GC.getCommerceInfo(COMMERCE_ESPIONAGE).getChar());
				szBuffer.append(szTempBuffer);
				break;
			}
			pSelectedUnitNode = gDLL->getInterfaceIFace()->nextSelectionListNode(pSelectedUnitNode);
		}
		break;
	}
	case MISSION_GOLDEN_AGE:
	{
		int iUnitConsume = kUnitOwner.unitsRequiredForGoldenAge();
		int iUnitDiff = (iUnitConsume - kUnitOwner.unitsGoldenAgeReady());
		if (iUnitDiff > 0)
		{
			szBuffer.append(NEWLINE);
			szBuffer.append(gDLL->getText("TXT_KEY_ACTION_MORE_GREAT_PEOPLE", iUnitDiff));
		}
		if (iUnitConsume > 1)
		{
			szBuffer.append(NEWLINE);
			szBuffer.append(gDLL->getText("TXT_KEY_ACTION_CONSUME_GREAT_PEOPLE", iUnitConsume));
		}
		break;
	}
	case MISSION_LEAD:
	{
		if (kUnit.getUnitInfo().getLeaderExperience() > 0)
		{
			int iNumUnits = kUnit.canGiveExperience(kUnit.plot());
			if (iNumUnits > 0)
			{
				szBuffer.append(NEWLINE);
				szBuffer.append(gDLL->getText("TXT_KEY_ACTION_LEAD_TROOPS",
						kUnit.getStackExperienceToGive(iNumUnits)));
			}
		}
		if (kUnit.getUnitInfo().getLeaderPromotion() != NO_PROMOTION)
		{
			szBuffer.append(NEWLINE);
			szBuffer.append(gDLL->getText("TXT_KEY_PROMOTION_WHEN_LEADING"));
			GAMETEXT.parsePromotionHelp(szBuffer, (PromotionTypes)
				kUnit.	getUnitInfo().getLeaderPromotion(), L"\n   ");
		}
		break;
	}
	case MISSION_ESPIONAGE:
	{
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_ACTION_ESPIONAGE_MISSION"));
		GAMETEXT.setEspionageMissionHelp(szBuffer, &kUnit);
		break;
	}
	case MISSION_BUILD:
	{
		BuildTypes eBuild = (BuildTypes)kAction.getMissionData();
		FAssert(eBuild != NO_BUILD);
		ImprovementTypes eImprovement = (ImprovementTypes)
				GC.getBuildInfo(eBuild).getImprovement();
		RouteTypes eRoute = (RouteTypes)GC.getBuildInfo(eBuild).getRoute();
		BonusTypes eBonus = kMissionPlot.getBonusType(kUnitTeam.getID());
		for (int iI = 0; iI < NUM_YIELD_TYPES; iI++)
		{
			YieldTypes eYield = (YieldTypes)iI;
			int iYield = 0;
			if (eImprovement != NO_IMPROVEMENT)
			{
				iYield += kMissionPlot.calculateImprovementYieldChange(eImprovement,
						eYield, kUnitOwner.getID());
				if (kMissionPlot.getImprovementType() != NO_IMPROVEMENT)
				{
					iYield -= kMissionPlot.calculateImprovementYieldChange(
							kMissionPlot.getImprovementType(), eYield,
							kUnitOwner.getID());
				}
			}
			if (NO_FEATURE != kMissionPlot.getFeatureType())
			{
				if (GC.getBuildInfo(eBuild).isFeatureRemove(kMissionPlot.getFeatureType())) {
					iYield -= GC.getFeatureInfo(kMissionPlot.getFeatureType()).
							getYieldChange(iI);
				}
			}
			if (iYield != 0)
			{
				szTempBuffer.Format(L", %s%d%c", ((iYield > 0) ? "+" : ""), iYield,
						GC.getYieldInfo((YieldTypes) iI).getChar());
				szBuffer.append(szTempBuffer);
			}
		}
		if (NO_IMPROVEMENT != eImprovement)
		{
			int iHappy = GC.getImprovementInfo(eImprovement).getHappiness();
			if (iHappy != 0)
			{
				szTempBuffer.Format(L", +%d%c", abs(iHappy),
						(iHappy > 0 ? gDLL->getSymbolID(HAPPY_CHAR) :
						gDLL->getSymbolID(UNHAPPY_CHAR)));
				szBuffer.append(szTempBuffer);
			}
		}
		bool bValid = false;
		pSelectedUnitNode = gDLL->getInterfaceIFace()->headSelectionListNode();
		while (pSelectedUnitNode != NULL)
		{
			pSelectedUnit = ::getUnit(pSelectedUnitNode->m_data);
			if (pSelectedUnit->canBuild(&kMissionPlot, eBuild))
			{
				bValid = true;
				break;
			}
			pSelectedUnitNode = gDLL->getInterfaceIFace()->nextSelectionListNode(
					pSelectedUnitNode);
		}
		if (!bValid)
		{
			if (eImprovement != NO_IMPROVEMENT)
			{
				CvImprovementInfo const& kImprov = GC.getImprovementInfo(eImprovement);
				if (kMissionPlot.getTeam() != kUnitTeam.getID())
				{
					if (kImprov.isOutsideBorders())
					{
						if (kMissionPlot.getTeam() != NO_TEAM)
						{
							szBuffer.append(NEWLINE);
							szBuffer.append(gDLL->getText("TXT_KEY_ACTION_NEEDS_OUT_RIVAL_CULTURE_BORDER"));
						}
					}
					else
					{
						szBuffer.append(NEWLINE);
						szBuffer.append(gDLL->getText("TXT_KEY_ACTION_NEEDS_CULTURE_BORDER"));
					}
				}
				if (eBonus == NO_BONUS || !kImprov.isImprovementBonusTrade(eBonus))
				{
					if (!kUnitTeam.isIrrigation() && !kUnitTeam.isIgnoreIrrigation())
					{
						if (kImprov.isRequiresIrrigation() &&
								!kMissionPlot.isIrrigationAvailable())
						{
							for (int iI = 0; iI < GC.getNumTechInfos(); iI++)
							{
								CvTechInfo const& kIrrigTech = GC.getTechInfo((TechTypes)iI);
								if (kIrrigTech.isIrrigation())
								{
									szBuffer.append(NEWLINE);
									szBuffer.append(gDLL->getText("TXT_KEY_BUILDING_REQUIRES_STRING",
											kIrrigTech.getTextKeyWide()));
									break;
								}
							}
						}
					}
				}
			}
			TechTypes eBuildPrereq = (TechTypes)GC.getBuildInfo(eBuild).getTechPrereq();
			if (!kUnitTeam.isHasTech(eBuildPrereq))
			{
				szBuffer.append(NEWLINE);
				szBuffer.append(gDLL->getText("TXT_KEY_BUILDING_REQUIRES_STRING",
						GC.getTechInfo(eBuildPrereq).getTextKeyWide()));
			}
			if (eRoute != NO_ROUTE)
			{
				BonusTypes eRoutePrereq = (BonusTypes)GC.getRouteInfo(eRoute).getPrereqBonus();
				if (eRoutePrereq != NO_BONUS)
				{
					if (!kMissionPlot.isAdjacentPlotGroupConnectedBonus(
							kUnitOwner.getID(), eRoutePrereq))
					{
						szBuffer.append(NEWLINE);
						szBuffer.append(gDLL->getText("TXT_KEY_BUILDING_REQUIRES_STRING",
							GC.getBonusInfo(eRoutePrereq).getTextKeyWide()));
					}
				}
				bool bFoundValid = true;
				std::vector<BonusTypes> aeOrBonuses;
				for (int i = 0; i < GC.getNUM_ROUTE_PREREQ_OR_BONUSES(); ++i)
				{
					BonusTypes eRoutePrereqOr = (BonusTypes)GC.getRouteInfo(eRoute).
							getPrereqOrBonus(i);
					if (NO_BONUS != eRoutePrereqOr)
					{
						aeOrBonuses.push_back(eRoutePrereqOr);
						bFoundValid = false;
						if (kMissionPlot.isAdjacentPlotGroupConnectedBonus(
								kUnitOwner.getID(), eRoutePrereqOr))
						{
							bFoundValid = true;
							break;
						}
					}
				}
				if (!bFoundValid)
				{
					bool bFirst = true;
					for (std::vector<BonusTypes>::iterator it = aeOrBonuses.begin(); it != aeOrBonuses.end(); ++it)
					{
						szFirstBuffer = NEWLINE +
								gDLL->getText("TXT_KEY_BUILDING_REQUIRES_LIST");
						szTempBuffer.Format(SETCOLR L"<link=literal>%s</link>" ENDCOLR,
								TEXT_COLOR("COLOR_HIGHLIGHT_TEXT"),
								GC.getBonusInfo(*it).getDescription());
						setListHelp(szBuffer, szFirstBuffer.GetCString(),
								szTempBuffer, gDLL->getText("TXT_KEY_OR").c_str(), bFirst);
						bFirst = false;
					}
				}
			}
			if (kMissionPlot.getFeatureType() != NO_FEATURE)
			{
				TechTypes eFeatureTech = (TechTypes)GC.getBuildInfo(eBuild).
						getFeatureTech(kMissionPlot.getFeatureType());
				if (!kUnitTeam.isHasTech(eFeatureTech))
				{
					szBuffer.append(NEWLINE);
					szBuffer.append(gDLL->getText("TXT_KEY_BUILDING_REQUIRES_STRING",
							GC.getTechInfo(eFeatureTech).getTextKeyWide()));
				}
			}
		}
		if (eImprovement != NO_IMPROVEMENT)
		{
			if (kMissionPlot.getImprovementType() != NO_IMPROVEMENT)
			{
				szBuffer.append(NEWLINE);
				szBuffer.append(gDLL->getText("TXT_KEY_ACTION_WILL_DESTROY_IMP",
						GC.getImprovementInfo(kMissionPlot.getImprovementType()).
						getTextKeyWide()));
			}
		}
		if (GC.getBuildInfo(eBuild).isKill())
		{
			szBuffer.append(NEWLINE);
			szBuffer.append(gDLL->getText("TXT_KEY_ACTION_CONSUME_UNIT"));
		}
		if (kMissionPlot.getFeatureType() != NO_FEATURE)
		{
			if (GC.getBuildInfo(eBuild).isFeatureRemove(kMissionPlot.getFeatureType()))
			{
				CvCity* pProductionCity=NULL;
				int iProduction = kMissionPlot.getFeatureProduction(eBuild,
						kUnitTeam.getID(), &pProductionCity);
				if (iProduction > 0)
				{
					szBuffer.append(NEWLINE);
					szBuffer.append(gDLL->getText("TXT_KEY_ACTION_CHANGE_PRODUCTION",
							iProduction, pProductionCity->getNameKey()));
				}
				szBuffer.append(NEWLINE);
				szBuffer.append(gDLL->getText("TXT_KEY_ACTION_REMOVE_FEATURE",
						GC.getFeatureInfo(kMissionPlot.getFeatureType()).getTextKeyWide()));
				// UNOFFICIAL_PATCH, Bugfix, 06/10/10, EmperorFool
				if (eImprovement == NO_IMPROVEMENT &&
						kMissionPlot.getImprovementType() != NO_IMPROVEMENT &&
						GC.getImprovementInfo(kMissionPlot.getImprovementType()).
						getFeatureMakesValid(kMissionPlot.getFeatureType()))
				{
					szBuffer.append(NEWLINE);
					szBuffer.append(gDLL->getText("TXT_KEY_ACTION_WILL_DESTROY_IMP",
							GC.getImprovementInfo(kMissionPlot.getImprovementType()).
							getTextKeyWide()));
				} // UNOFFICIAL_PATCH: END
			}
		}
		if (eImprovement != NO_IMPROVEMENT)
		{
			CvImprovementInfo const& kImprov = GC.getImprovementInfo(eImprovement);
			if (eBonus != NO_BONUS)
			{
				//if (!kUnitTeam.isBonusObsolete(eBonus))
				if (kUnitOwner.doesImprovementConnectBonus(eImprovement, eBonus)) // K-Mod
				{ //if (kImprov.isImprovementBonusTrade(eBonus))
					CvBonusInfo const& kBonus = GC.getBonusInfo(eBonus);
					szBuffer.append(NEWLINE);
					szBuffer.append(gDLL->getText("TXT_KEY_ACTION_PROVIDES_BONUS",
							kBonus.getTextKeyWide()));
					if (kBonus.getHealth() != 0)
					{
						szTempBuffer.Format(L" (+%d%c)", abs(kBonus.getHealth()),
								(kBonus.getHealth() > 0 ?
								gDLL->getSymbolID(HEALTHY_CHAR) :
								gDLL->getSymbolID(UNHEALTHY_CHAR)));
						szBuffer.append(szTempBuffer);
					}
					if (kBonus.getHappiness() != 0)
					{
						szTempBuffer.Format(L" (+%d%c)", abs(kBonus.getHappiness()),
								(kBonus.getHappiness() > 0 ?
								gDLL->getSymbolID(HAPPY_CHAR) :
								gDLL->getSymbolID(UNHAPPY_CHAR)));
						szBuffer.append(szTempBuffer);
					}
				}
			}
			else
			{
				int iLast = 0;
				FAssertMsg(0 < GC.getNumBonusInfos(), "GC.getNumBonusInfos() is negative but an array is being allocated in CvDLLWidgetData::parseActionHelp_Mission");
				for (int iI = 0; iI < GC.getNumBonusInfos(); iI++)
				{
					BonusTypes eRandBonus = (BonusTypes)iI;
					if (kUnitTeam.isHasTech((TechTypes)(GC.getBonusInfo(eRandBonus).
							getTechReveal())))
					{
						if (kImprov.getImprovementBonusDiscoverRand(iI) > 0
								// advc.rom3:
								&& kMissionPlot.canHaveBonus(eRandBonus, false,
								true)) // advc.129
						{
							szFirstBuffer.Format(L"%s%s", NEWLINE,
									gDLL->getText("TXT_KEY_ACTION_CHANCE_DISCOVER").c_str());
							szTempBuffer.Format(L"%c", GC.getBonusInfo(eRandBonus).getChar());
							setListHelp(szBuffer, szFirstBuffer, szTempBuffer, L", ",
									kImprov.getImprovementBonusDiscoverRand(iI) != iLast);
							iLast = kImprov.getImprovementBonusDiscoverRand(iI);
						}
					}
				}
			}
			if (!kMissionPlot.isIrrigationAvailable())
			{
				GAMETEXT.setYieldChangeHelp(szBuffer,
						gDLL->getText("TXT_KEY_ACTION_IRRIGATED").c_str(), L": ", L"",
						GC.getImprovementInfo(eImprovement).getIrrigatedYieldChangeArray());
			}
			if (eRoute == NO_ROUTE)
			{
				for (int iI = 0; iI < GC.getNumRouteInfos(); iI++)
				{
					RouteTypes eLoopRoute = (RouteTypes)iI;
					if (kMissionPlot.getRouteType() != eLoopRoute)
					{
						GAMETEXT.setYieldChangeHelp(szBuffer,
								GC.getRouteInfo(eLoopRoute).getDescription(), L": ", L"",
								kImprov.getRouteYieldChangesArray(eLoopRoute));
					}
				}
			}
			if (kImprov.getDefenseModifier() != 0)
			{
				szBuffer.append(NEWLINE);
				szBuffer.append(gDLL->getText("TXT_KEY_ACTION_DEFENSE_MODIFIER",
						kImprov.getDefenseModifier()));
			}
			ImprovementTypes eUpgr = (ImprovementTypes)kImprov.getImprovementUpgrade();
			if (eUpgr != NO_IMPROVEMENT)
			{
				int iTurns = kMissionPlot.getUpgradeTimeLeft(eImprovement,
						kUnitOwner.getID());
				szBuffer.append(NEWLINE);
				szBuffer.append(gDLL->getText("TXT_KEY_ACTION_BECOMES_IMP",
						GC.getImprovementInfo(eUpgr).getTextKeyWide(), iTurns));
			}
		}
		if (eRoute != NO_ROUTE)
		{
			ImprovementTypes eFinalImprovement = eImprovement;
			if (eFinalImprovement == NO_IMPROVEMENT)
				eFinalImprovement = kMissionPlot.getImprovementType();
			if (eFinalImprovement != NO_IMPROVEMENT)
			{
				GAMETEXT.setYieldChangeHelp(szBuffer, GC.getImprovementInfo(
						eFinalImprovement).getDescription(), L": ", L"",
						GC.getImprovementInfo(eFinalImprovement).
						getRouteYieldChangesArray(eRoute));
			}
			int iMovementCost = GC.getRouteInfo(eRoute).getMovementCost() +
					kUnitTeam.getRouteChange(eRoute);
			int iFlatMovementCost = GC.getRouteInfo(eRoute).getFlatMovementCost();
			int iMoves = GC.getMOVE_DENOMINATOR();
			if (iMovementCost > 0)
			{
				iMoves /= iMovementCost;
				if (iMoves * iMovementCost < GC.getMOVE_DENOMINATOR())
					iMoves++;
			}
			int iFlatMoves = GC.getMOVE_DENOMINATOR();
			if (iFlatMovementCost > 0)
			{
				iFlatMoves /= iFlatMovementCost;
				if (iFlatMoves * iFlatMovementCost < GC.getMOVE_DENOMINATOR())
					iFlatMoves++;
			}
			if (iMoves > 1 || iFlatMoves > 1)
			{
				if (iMoves >= iFlatMoves)
				{
					szBuffer.append(NEWLINE);
					szBuffer.append(gDLL->getText("TXT_KEY_ACTION_MOVEMENT_COST", iMoves));
				}
				else
				{
					szBuffer.append(NEWLINE);
					szBuffer.append(gDLL->getText("TXT_KEY_ACTION_FLAT_MOVEMENT_COST",
							iFlatMoves));
				}
			}
			szBuffer.append(NEWLINE);
			szBuffer.append(gDLL->getText("TXT_KEY_ACTION_CONNECTS_RESOURCES"));
		}
		int iNowWorkRate = 0;
		int iThenWorkRate = 0;
		pSelectedUnitNode = gDLL->getInterfaceIFace()->headSelectionListNode();
		int iBuildCost = kUnitOwner.getBuildCost(&kMissionPlot, eBuild);
		if (iBuildCost > 0)
		{
			szBuffer.append(NEWLINE);
			szBuffer.append(gDLL->getText("TXT_KEY_BUILD_COST", iBuildCost));
		}
		while (pSelectedUnitNode != NULL)
		{
			pSelectedUnit = ::getUnit(pSelectedUnitNode->m_data);
			if (pSelectedUnit->getBuildType() != eBuild)
			{
				iNowWorkRate += pSelectedUnit->workRate(false);
				iThenWorkRate += pSelectedUnit->workRate(true);
			}
			pSelectedUnitNode = gDLL->getInterfaceIFace()->nextSelectionListNode(
					pSelectedUnitNode);
		}
		int iTurns = kMissionPlot.getBuildTurnsLeft(eBuild,
				kUnitOwner.getID(), // advc.251
				iNowWorkRate, iThenWorkRate);
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_ACTION_NUM_TURNS", iTurns));
		CvBuildInfo const& kBuild = GC.getBuildInfo(eBuild);
		if (!CvWString(kBuild.getHelp()).empty())
		{
			szBuffer.append(NEWLINE);
			szBuffer.append(kBuild.getHelp());
		} // <advc.011b>
		if(bValid && iTurns > 1 && GC.ctrlKey()) {
			szBuffer.append(NEWLINE);
			szBuffer.append(gDLL->getText("TXT_KEY_SUSPEND_WORK"));
		} // </advc.011b>
		break; // advc.003: Last case of switch(eMission)
	}
	} // end of switch

	if (!CvWString(GC.getMissionInfo(eMission).getHelp()).empty())
	{	// <advc.004a>
		if(eMission == MISSION_DISCOVER)
			szBuffer.append(getDiscoverPathText(kUnit.getUnitType(), kUnitOwner.getID()));
		else { // </advc.004a>
			szBuffer.append(NEWLINE);
			szBuffer.append(GC.getMissionInfo(eMission).getHelp());
		}
	}
}


void CvDLLWidgetData::parseCitizenHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)  // advc.003: style changes
{
	CvCity* pHeadSelectedCity = gDLL->getInterfaceIFace()->getHeadSelectedCity();
	if (pHeadSelectedCity == NULL || widgetDataStruct.m_iData1 == NO_SPECIALIST)
		return;

	GAMETEXT.parseSpecialistHelp(szBuffer, (SpecialistTypes)
			widgetDataStruct.m_iData1, pHeadSelectedCity);

	if (widgetDataStruct.m_iData2 == -1)
		return;

	int iCount = 0;
	for (int iI = 0; iI < GC.getNumSpecialistInfos(); iI++)
	{
		if (iI < widgetDataStruct.m_iData1)
			iCount += pHeadSelectedCity->getSpecialistCount((SpecialistTypes)iI);
		else if (iI == widgetDataStruct.m_iData1)
			iCount += widgetDataStruct.m_iData2;
	}
	if (iCount < pHeadSelectedCity->totalFreeSpecialists())
	{
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_MISC_FREE_SPECIALIST"));
	}
}


void CvDLLWidgetData::parseFreeCitizenHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	CvCity* pHeadSelectedCity = gDLL->getInterfaceIFace()->getHeadSelectedCity();
	if (pHeadSelectedCity == NULL)
		return; // advc.003

	SpecialistTypes eSpecialist = (SpecialistTypes)widgetDataStruct.m_iData1;
	if (NO_SPECIALIST != eSpecialist)
		GAMETEXT.parseSpecialistHelp(szBuffer, eSpecialist, pHeadSelectedCity);
	if (widgetDataStruct.m_iData2 != -1)
	{
		szBuffer.append(SEPARATOR);
		GAMETEXT.parseFreeSpecialistHelp(szBuffer, *pHeadSelectedCity);
	}
}


void CvDLLWidgetData::parseDisabledCitizenHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)  // advc.003: style changes
{
	CvCity* pHeadSelectedCity = gDLL->getInterfaceIFace()->getHeadSelectedCity();
	if (pHeadSelectedCity == NULL || widgetDataStruct.m_iData1 == NO_SPECIALIST)
		return;

	GAMETEXT.parseSpecialistHelp(szBuffer, (SpecialistTypes)
			widgetDataStruct.m_iData1, pHeadSelectedCity);

	if (pHeadSelectedCity->isSpecialistValid((SpecialistTypes)
			widgetDataStruct.m_iData1, 1))
		return;

	CvWString szTempBuffer;
	bool bFirst = true;

	for (int iI = 0; iI < GC.getNumBuildingClassInfos(); iI++)
	{
		BuildingTypes eLoopBuilding = (BuildingTypes)GC.getCivilizationInfo(GC.getGameINLINE().getActiveCivilizationType()).getCivilizationBuildings(iI);
		if (eLoopBuilding == NO_BUILDING)
			continue;

		if (GC.getBuildingInfo(eLoopBuilding).getSpecialistCount(widgetDataStruct.m_iData1) > 0)
		{
			if ((pHeadSelectedCity->getNumBuilding(eLoopBuilding) <= 0) &&
					!isLimitedWonderClass((BuildingClassTypes)iI))
			{
				if (GC.getBuildingInfo(eLoopBuilding).getSpecialBuildingType() == NO_SPECIALBUILDING ||
						pHeadSelectedCity->canConstruct(eLoopBuilding))
				{
					szTempBuffer = NEWLINE + gDLL->getText("TXT_KEY_REQUIRES");
					setListHelp(szBuffer, szTempBuffer, GC.getBuildingInfo(
							eLoopBuilding).getDescription(),
							gDLL->getText("TXT_KEY_OR").c_str(), bFirst);
					bFirst = false;
				}
			}
		}
	}

	if (!bFirst)
		szBuffer.append(ENDCOLR);
}


void CvDLLWidgetData::parseAngryCitizenHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	CvCity* pHeadSelectedCity = gDLL->getInterfaceIFace()->getHeadSelectedCity();
	if (pHeadSelectedCity != NULL)
	{
		szBuffer.assign(gDLL->getText("TXT_KEY_MISC_ANGRY_CITIZEN"));
		szBuffer.append(NEWLINE);

		GAMETEXT.setAngerHelp(szBuffer, *pHeadSelectedCity);
	}
}


void CvDLLWidgetData::parseChangeSpecialistHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)  // advc.003: style changes
{
	CvCity* pHeadSelectedCity = gDLL->getInterfaceIFace()->getHeadSelectedCity();

	if (pHeadSelectedCity == NULL)
		return;

	if (widgetDataStruct.m_iData2 > 0)
	{
		GAMETEXT.parseSpecialistHelp(szBuffer, (SpecialistTypes)widgetDataStruct.m_iData1, pHeadSelectedCity);
		if (widgetDataStruct.m_iData1 != GC.getDefineINT("DEFAULT_SPECIALIST"))
		{
			if (!GET_PLAYER(pHeadSelectedCity->getOwnerINLINE()).
					isSpecialistValid((SpecialistTypes)widgetDataStruct.m_iData1))
			{
				if (pHeadSelectedCity->getMaxSpecialistCount((SpecialistTypes)
						widgetDataStruct.m_iData1) > 0)
				{
					szBuffer.append(NEWLINE);
					szBuffer.append(gDLL->getText("TXT_KEY_MISC_MAX_SPECIALISTS",
							pHeadSelectedCity->getMaxSpecialistCount(
							(SpecialistTypes)widgetDataStruct.m_iData1)));
				}
			}
		}
	}
	else
	{
		szBuffer.assign(gDLL->getText("TXT_KEY_MISC_REMOVE_SPECIALIST", GC.getSpecialistInfo(
				(SpecialistTypes)widgetDataStruct.m_iData1).getTextKeyWide()));

		if (pHeadSelectedCity->getForceSpecialistCount((SpecialistTypes)widgetDataStruct.m_iData1) > 0)
		{
			szBuffer.append(NEWLINE);
			szBuffer.append(gDLL->getText("TXT_KEY_MISC_FORCED_SPECIALIST",
					pHeadSelectedCity->getForceSpecialistCount((SpecialistTypes)
					widgetDataStruct.m_iData1)));
		}
	}
}


void CvDLLWidgetData::parseResearchHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	TechTypes eTech = (TechTypes)widgetDataStruct.m_iData1;
	if (eTech == NO_TECH)
	{
		//	No Technology
		if (GET_PLAYER(GC.getGameINLINE().getActivePlayer()).getCurrentResearch() != NO_TECH)
		{
			CvGameAI& game = GC.getGameINLINE();
			CvPlayer& activePlayer = GET_PLAYER(game.getActivePlayer());
			szBuffer.assign(gDLL->getText("TXT_KEY_MISC_CHANGE_RESEARCH"));
			szBuffer.append(NEWLINE);
			GAMETEXT.setTechHelp(szBuffer, activePlayer.getCurrentResearch(), false, true);
		}
	}
	else
	{
		GAMETEXT.setTechHelp(szBuffer, eTech, false, true, widgetDataStruct.m_bOption);
	}
}


void CvDLLWidgetData::parseTechTreeHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	TechTypes eTech = (TechTypes)widgetDataStruct.m_iData1;
	GAMETEXT.setTechHelp(szBuffer, eTech, false,
			// advc.004: bPlayerContext:
			!GET_TEAM(GC.getGameINLINE().getActiveTeam()).isHasTech(eTech),
			false, false);
}


void CvDLLWidgetData::parseChangePercentHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	if (widgetDataStruct.m_iData2 > 0)
	{
		szBuffer.assign(gDLL->getText("TXT_KEY_MISC_INCREASE_RATE", GC.getCommerceInfo((CommerceTypes) widgetDataStruct.m_iData1).getTextKeyWide(), widgetDataStruct.m_iData2));
	}
	else
	{
		szBuffer.assign(gDLL->getText("TXT_KEY_MISC_DECREASE_RATE", GC.getCommerceInfo((CommerceTypes) widgetDataStruct.m_iData1).getTextKeyWide(), -(widgetDataStruct.m_iData2)));
	}
}

// advc (comment): Could this function be merged into CvGameTextMgr::parseLeaderHeadHelp?
void CvDLLWidgetData::parseContactCivHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)  // advc.003: Some style changes
{
	PlayerTypes ePlayer = (PlayerTypes)widgetDataStruct.m_iData1;
	// do not execute if player is not a real civ
	CvPlayerAI const& kPlayer = GET_PLAYER(ePlayer);
	if (kPlayer.getCivilizationType() == NO_CIVILIZATION)
		return;
	// make sure its empty to start
	szBuffer.clear();

	TeamTypes eTeam = kPlayer.getTeam();
	PlayerTypes eActivePlayer = GC.getGameINLINE().getActivePlayer();
	TeamTypes eActiveTeam = GET_PLAYER(eActivePlayer).getTeam();
	CvTeamAI const& kActiveTeam = GET_TEAM(eActiveTeam);

	// if alt down and cheat on, show extra info
	if (GC.altKey() && //gDLL->getChtLvl() > 0)
			GC.getGameINLINE().isDebugMode()) // advc.135c
	{
		// K-Mod. I've moved the code from here into its own function, just to get it out of the way.
		parseScoreboardCheatText(widgetDataStruct, szBuffer);
		// K-Mod end
	}
	// Show score info instead if we are trying to contact ourselves...
	/*  advc.085: No, show active player's contact hover, mainly, for the worst-enemy info.
		(And I've moved these checks into Scoreboard.py.) */
	/*if(eActivePlayer == ePlayer || (GC.ctrlKey() && GC.getGameINLINE().isDebugMode())) {
		parseScoreHelp(widgetDataStruct, szBuffer);
		return;
	}*/

	/* original bts code
	szBuffer.append(gDLL->getText("TXT_KEY_MISC_CONTACT_LEADER", kPlayer.getNameKey(), kPlayer.getCivilizationShortDescription()));
	szBuffer.append(NEWLINE);
	GAMETEXT.parsePlayerTraits(szBuffer, ePlayer); */
	if (eActivePlayer != ePlayer && // advc.085
			!kActiveTeam.isHasMet(eTeam))
	{	// K-Mod. If we haven't met the player yet - don't say "contact". Because we can't actually contact them!
		szBuffer.append(CvWString::format(SETCOLR L"%s" ENDCOLR,
				TEXT_COLOR("COLOR_HIGHLIGHT_TEXT"), kPlayer.getName()));
		// K-Mod end
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_MISC_HAVENT_MET_CIV"));
	}
	// K-Mod
	else
	{
		CvWString szTmp; // advc.085
		szTmp.append(gDLL->getText(
				eActivePlayer == ePlayer ? "TXT_KEY_LEADER_CIV_DESCRIPTION" : // advc.085
				"TXT_KEY_MISC_CONTACT_LEADER",
				kPlayer.getNameKey(), kPlayer.getCivilizationShortDescription()));
		// <advc.085>
		if(eActivePlayer == ePlayer) {
			szBuffer.append(CvWString::format(SETCOLR L"%s" ENDCOLR,
					TEXT_COLOR("COLOR_HIGHLIGHT_TEXT"), szTmp.GetCString()));
		}
		else szBuffer.append(szTmp); // </advc.085>
		szBuffer.append(NEWLINE);
		GAMETEXT.parsePlayerTraits(szBuffer, ePlayer);
	} // K-Mod end

	if (kActiveTeam.isHasMet(eTeam) || GC.getGameINLINE().isDebugMode())
	{	/* original bts code
		if (!kPlayer.isHuman()) {
			if (!kPlayer.AI_isWillingToTalk(eActivePlayer)) {
				szBuffer.append(NEWLINE);
				szBuffer.append(gDLL->getText("TXT_KEY_MISC_REFUSES_TO_TALK"));
			}
			if (!((GC.altKey() || GC.ctrlKey()) && gDLL->getChtLvl() > 0)) {
				szBuffer.append(NEWLINE);
				GAMETEXT.getAttitudeString(szBuffer, ePlayer, eActivePlayer);
				szBuffer.append(NEWLINE);
				GAMETEXT.getEspionageString(szBuffer, ((PlayerTypes)widgetDataStruct.m_iData1), eActivePlayer);
				szBuffer.append(gDLL->getText("TXT_KEY_MISC_CTRL_TRADE"));
			}
		}
		else {
			szBuffer.append(NEWLINE);
			GAMETEXT.getEspionageString(szBuffer, ((PlayerTypes)widgetDataStruct.m_iData1), eActivePlayer);
		}*/
		bool bWillTalk = (eActivePlayer == ePlayer || // advc.085
				kPlayer.AI_isWillingToTalk(eActivePlayer, /* advc.104l: */ true));
		// K-Mod
		if (!bWillTalk)
		{
			szBuffer.append(NEWLINE);
			szBuffer.append(gDLL->getText("TXT_KEY_MISC_REFUSES_TO_TALK"));
		}
		if (!((GC.altKey() || GC.ctrlKey()) && //gDLL->getChtLvl() > 0))
			GC.getGameINLINE().isDebugMode())) // advc.135c
		{
			if(eActivePlayer != ePlayer) // advc.085
				GAMETEXT.getAttitudeString(szBuffer, ePlayer, eActivePlayer);
			GAMETEXT.getWarWearinessString(szBuffer, ePlayer, // K-Mod
					eActivePlayer == ePlayer ? NO_PLAYER : // advc.085
					eActivePlayer);
			// <advc.104v> Handled later
			/*if (!kPlayer.isHuman() && willTalk) {
				szBuffer.append(NEWLINE);
				szBuffer.append(gDLL->getText("TXT_KEY_MISC_CTRL_TRADE"));
			}*/
		} // K-Mod end
		// <advc.034>
		if(GET_TEAM(eActiveTeam).isDisengage(eTeam))
		{
			CvWString szString;
			GAMETEXT.buildDisengageString(szString, eActivePlayer, ePlayer);
			szBuffer.append(NEWLINE);
			szBuffer.append(szString);
		} // </advc.034>
		//if (eTeam != eActiveTeam ) // advc.085
		// Show which civs this player is at war with
		CvWStringBuffer szWarWithString;
		CvWStringBuffer szWorstEnemyString;
		bool bFirst = true;
		bool bFirst2 = true;
		// advc.003: Variables renamed to ...Loop... in order to avoid shadowing
		for (int iLoopTeam = 0; iLoopTeam < MAX_CIV_TEAMS; iLoopTeam++)
		{
			CvTeamAI& kLoopTeam = GET_TEAM((TeamTypes)iLoopTeam);
			if (!kLoopTeam.isAlive() || kLoopTeam.isMinorCiv() || iLoopTeam == eTeam)
					// K-Mod. show "at war" for the active player if appropriate
					//|| iLoopTeam == TEAMID(ePlayer))
				continue;
			if (!kActiveTeam.isHasMet(kLoopTeam.getID()))
				continue;
			if (::atWar((TeamTypes)iLoopTeam, eTeam))
			{
				setListHelp(szWarWithString, L"", kLoopTeam.getName().GetCString(), L", ", bFirst);
				bFirst = false;
			}
			//if (kTeam.AI_getWorstEnemy() == GET_PLAYER(ePlayer).getTeam())
			if (!kLoopTeam.isHuman() && kLoopTeam.AI_getWorstEnemy() == eTeam) // K-Mod
			{
				setListHelp(szWorstEnemyString, L"", kLoopTeam.getName().GetCString(), L", ", bFirst2);
				bFirst2 = false;
			}
		}
		if (!szWarWithString.isEmpty()) // advc.004: List wars before worst enemies
		{
			szBuffer.append(NEWLINE);
			szBuffer.append(gDLL->getText(L"TXT_KEY_AT_WAR_WITH", szWarWithString.getCString()));
		}
		if (!szWorstEnemyString.isEmpty())
		{
			szBuffer.append(NEWLINE);
			szBuffer.append(gDLL->getText(L"TXT_KEY_WORST_ENEMY_OF", szWorstEnemyString.getCString()));
		}
		// <advc.004v> Moved here from above
		bool bShowCtrlTrade = (!((GC.altKey() || GC.ctrlKey()) && //gDLL->getChtLvl() > 0)
				GC.getGameINLINE().isDebugMode()) // advc.135c
				&& !kPlayer.isHuman() && bWillTalk
				&& ePlayer != eActivePlayer); // advc.085
		if (bShowCtrlTrade) {
			szBuffer.append(NEWLINE);
			szBuffer.append(gDLL->getText("TXT_KEY_MISC_CTRL_TRADE"));
		} // </advc.004>
		if (!kActiveTeam.isAtWar(eTeam) /* advc.085: */ && ePlayer != eActivePlayer)
		{
			if (kActiveTeam.canDeclareWar(eTeam))
			{	// <advc.104v>
				if(bShowCtrlTrade)
					szBuffer.append(L", "); // Put them on one line
				else // </advc.104v>
					szBuffer.append(NEWLINE);
				szBuffer.append(gDLL->getText("TXT_KEY_MISC_ALT_DECLARE_WAR"));
			}
			else
			{
				szBuffer.append(NEWLINE);
				szBuffer.append(gDLL->getText("TXT_KEY_MISC_CANNOT_DECLARE_WAR"));
			}
			// K-Mod. The BBAI war plan control currently is not implemented for multiplayer, and it is only relevant for team games.
			if (!GC.getGameINLINE().isGameMultiPlayer() && kActiveTeam.getAliveCount() > 1)
			{
				szBuffer.append(NEWLINE);
				szBuffer.append(gDLL->getText("TXT_KEY_MISC_SHIFT_ALT_PREPARE_WAR"));
			}
		}
	}

	if (kPlayer.isHuman() /* advc.085: */ && eActivePlayer != ePlayer)
	{
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_MISC_SHIFT_SEND_CHAT"));
	}
}

// K-Mod. The cheat mode text associated with parseContactCivHelp.
void CvDLLWidgetData::parseScoreboardCheatText(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	PlayerTypes ePlayer = (PlayerTypes) widgetDataStruct.m_iData1;

	const CvPlayerAI& kPlayer = GET_PLAYER(ePlayer);

	TeamTypes eTeam = (TeamTypes) kPlayer.getTeam();
	const CvTeamAI& kTeam = GET_TEAM(eTeam);

	PlayerTypes eActivePlayer = GC.getGameINLINE().getActivePlayer();

	// Show tech percent adjust
	szBuffer.append(CvWString::format(SETCOLR L"TechPercent: %d%%, CurResMod: %d%%" ENDCOLR, TEXT_COLOR("COLOR_HIGHLIGHT_TEXT"), kTeam.getBestKnownTechScorePercent(), kPlayer.calculateResearchModifier(kPlayer.getCurrentResearch()) ));
	szBuffer.append(NEWLINE);
	szBuffer.append(NEWLINE);

	szBuffer.append("Power");
	szBuffer.append(NEWLINE);
	// show everyones power for the active player
	if (eActivePlayer == ePlayer)
	{
		for (int iI = 0; iI < MAX_PLAYERS; iI++)
		{
			CvPlayer& kLoopPlayer = GET_PLAYER((PlayerTypes)iI);
			if (kLoopPlayer.isAlive())
			{
				CvTeam& kLoopTeam = GET_TEAM((TeamTypes) kLoopPlayer.getTeam());

				szBuffer.append(CvWString::format(SETCOLR L"%s (team%d): %d (%d def)" ENDCOLR, TEXT_COLOR("COLOR_HIGHLIGHT_TEXT"),
					kLoopPlayer.getName(), kLoopPlayer.getTeam(), kLoopPlayer.getPower(), kLoopTeam.getDefensivePower()));
				
				// if in financial trouble, show that
				if (kLoopPlayer.AI_isFinancialTrouble())
				{
					szBuffer.append(CvWString::format(SETCOLR L" ($$$)" ENDCOLR, TEXT_COLOR("COLOR_NEGATIVE_TEXT")));
				}

				szBuffer.append(NEWLINE);
			}
		}
	}
	// only should this one power if not active player
	else
	{
		szBuffer.append(CvWString::format(SETCOLR L"%d (%d) power" ENDCOLR, TEXT_COLOR("COLOR_HIGHLIGHT_TEXT"), kPlayer.getPower(), kTeam.getPower(true)));
		
		// if in financial trouble, show that
		if (kPlayer.AI_isFinancialTrouble())
		{
			szBuffer.append(CvWString::format(SETCOLR L" ($$$)" ENDCOLR, TEXT_COLOR("COLOR_NEGATIVE_TEXT")));
		}

		szBuffer.append(NEWLINE);
	}

	// Strategies
	CvWString szTempBuffer;
	szTempBuffer.Format(L"");

	//kPlayer.AI_getStrategyHash();
	if (kPlayer.AI_isDoStrategy(AI_STRATEGY_DAGGER))
    {
        szTempBuffer.Format(L"Dagger, ");
        szBuffer.append(szTempBuffer);
    }
	if (kPlayer.AI_isDoStrategy(AI_STRATEGY_CRUSH))
    {
        szTempBuffer.Format(L"Crush, ");
        szBuffer.append(szTempBuffer);
	}
	if (kPlayer.AI_isDoStrategy(AI_STRATEGY_ALERT1))
    {
        szTempBuffer.Format(L"Alert1, ");
        szBuffer.append(szTempBuffer);
	}
	if (kPlayer.AI_isDoStrategy(AI_STRATEGY_ALERT2))
    {
        szTempBuffer.Format(L"Alert2, ");
        szBuffer.append(szTempBuffer);
	}
	if (kPlayer.AI_isDoStrategy(AI_STRATEGY_TURTLE))
    {
        szTempBuffer.Format(L"Turtle, ");
        szBuffer.append(szTempBuffer);
	}
	if (kPlayer.AI_isDoStrategy(AI_STRATEGY_LAST_STAND))
    {
        szTempBuffer.Format(L"LastStand, ");
        szBuffer.append(szTempBuffer);
	}
	if (kPlayer.AI_isDoStrategy(AI_STRATEGY_FINAL_WAR))
    {
        szTempBuffer.Format(L"FinalWar, ");
        szBuffer.append(szTempBuffer);
    }
    if (kPlayer.AI_isDoStrategy(AI_STRATEGY_GET_BETTER_UNITS))
    {
        szTempBuffer.Format(L"GetBetterUnits, ");
        szBuffer.append(szTempBuffer);
    }
	if (kPlayer.AI_isDoStrategy(AI_STRATEGY_PRODUCTION))
    {
        szTempBuffer.Format(L"Production, ");
        szBuffer.append(szTempBuffer);
    }
	if (kPlayer.AI_isDoStrategy(AI_STRATEGY_MISSIONARY))
    {
        szTempBuffer.Format(L"Missionary, ");
        szBuffer.append(szTempBuffer);
    }
	if (kPlayer.AI_isDoStrategy(AI_STRATEGY_BIG_ESPIONAGE))
    {
        szTempBuffer.Format(L"BigEspionage, ");
        szBuffer.append(szTempBuffer);
    }
	if (kPlayer.AI_isDoStrategy(AI_STRATEGY_ECONOMY_FOCUS)) // K-Mod
	{
		szTempBuffer.Format(L"EconomyFocus, ");
		szBuffer.append(szTempBuffer);
	}
	if (kPlayer.AI_isDoStrategy(AI_STRATEGY_ESPIONAGE_ECONOMY)) // K-Mod
	{
		szTempBuffer.Format(L"EspionageEconomy, ");
		szBuffer.append(szTempBuffer);
	}

	szBuffer.append(NEWLINE);
	szTempBuffer.Format(L"Vic Strats: ");
    szBuffer.append(szTempBuffer);

	szTempBuffer.Format(L"");
	// Victory strategies

	kPlayer.AI_getVictoryStrategyHash();
	/*  <advc.007> Reordered and "else" added so that only the highest stage is
		displayed. */
	if (kPlayer.AI_isDoVictoryStrategy(AI_VICTORY_CULTURE4))
    {
        szTempBuffer.Format(L"Culture4, ");
        szBuffer.append(szTempBuffer);
    }
    else if (kPlayer.AI_isDoVictoryStrategy(AI_VICTORY_CULTURE3))
    {
        szTempBuffer.Format(L"Culture3, ");
        szBuffer.append(szTempBuffer);
    }
    else if (kPlayer.AI_isDoVictoryStrategy(AI_VICTORY_CULTURE2))
    {
        szTempBuffer.Format(L"Culture2, ");
        szBuffer.append(szTempBuffer);
    }
	else if (kPlayer.AI_isDoVictoryStrategy(AI_VICTORY_CULTURE1))
    {
        szTempBuffer.Format(L"Culture1, ");
        szBuffer.append(szTempBuffer);
    }

	if (kPlayer.AI_isDoVictoryStrategy(AI_VICTORY_SPACE4))
    {
        szTempBuffer.Format(L"Space4, ");
        szBuffer.append(szTempBuffer);
    }
    else if (kPlayer.AI_isDoVictoryStrategy(AI_VICTORY_SPACE3))
    {
        szTempBuffer.Format(L"Space3, ");
        szBuffer.append(szTempBuffer);
    }
    else if (kPlayer.AI_isDoVictoryStrategy(AI_VICTORY_SPACE2))
    {
        szTempBuffer.Format(L"Space2, ");
        szBuffer.append(szTempBuffer);
    }
	else if (kPlayer.AI_isDoVictoryStrategy(AI_VICTORY_SPACE1))
    {
        szTempBuffer.Format(L"Space1, ");
        szBuffer.append(szTempBuffer);
    }

	if (kPlayer.AI_isDoVictoryStrategy(AI_VICTORY_CONQUEST4))
    {
        szTempBuffer.Format(L"Conq4, ");
        szBuffer.append(szTempBuffer);
    }
	else if (kPlayer.AI_isDoVictoryStrategy(AI_VICTORY_CONQUEST3))
    {
        szTempBuffer.Format(L"Conq3, ");
        szBuffer.append(szTempBuffer);
    }
	else if (kPlayer.AI_isDoVictoryStrategy(AI_VICTORY_CONQUEST2))
    {
        szTempBuffer.Format(L"Conq2, ");
        szBuffer.append(szTempBuffer);
    }
	else if (kPlayer.AI_isDoVictoryStrategy(AI_VICTORY_CONQUEST1))
    {
        szTempBuffer.Format(L"Conq1, ");
        szBuffer.append(szTempBuffer);
    }

	if (kPlayer.AI_isDoVictoryStrategy(AI_VICTORY_DOMINATION4))
    {
        szTempBuffer.Format(L"Dom4, ");
        szBuffer.append(szTempBuffer);
    }
	else if (kPlayer.AI_isDoVictoryStrategy(AI_VICTORY_DOMINATION3))
    {
        szTempBuffer.Format(L"Dom3, ");
        szBuffer.append(szTempBuffer);
    }
    else if (kPlayer.AI_isDoVictoryStrategy(AI_VICTORY_DOMINATION2))
    {
        szTempBuffer.Format(L"Dom2, ");
        szBuffer.append(szTempBuffer);
    }
	else if (kPlayer.AI_isDoVictoryStrategy(AI_VICTORY_DOMINATION1))
    {
        szTempBuffer.Format(L"Dom1, ");
        szBuffer.append(szTempBuffer);
    }

	if (kPlayer.AI_isDoVictoryStrategy(AI_VICTORY_DIPLOMACY4))
    {
        szTempBuffer.Format(L"Diplo4, ");
        szBuffer.append(szTempBuffer);
    }
	else if (kPlayer.AI_isDoVictoryStrategy(AI_VICTORY_DIPLOMACY3))
    {
        szTempBuffer.Format(L"Diplo3, ");
        szBuffer.append(szTempBuffer);
    }
    else if (kPlayer.AI_isDoVictoryStrategy(AI_VICTORY_DIPLOMACY2))
    {
        szTempBuffer.Format(L"Diplo2, ");
        szBuffer.append(szTempBuffer);
    }
	else if (kPlayer.AI_isDoVictoryStrategy(AI_VICTORY_DIPLOMACY1))
    {
        szTempBuffer.Format(L"Diplo1, ");
        szBuffer.append(szTempBuffer);
    } // </advc.007>
	
	// List the top 3 culture cities (by culture value weight).
	//if (kPlayer.AI_isDoVictoryStrategy(AI_VICTORY_CULTURE1))
	/*  advc.007: The line above was already commented out; i.e. culture info was
		always shown. */
	if (kPlayer.AI_isDoVictoryStrategy(AI_VICTORY_CULTURE3) || GC.ctrlKey())
	{
		szBuffer.append(CvWString::format(L"\n\nTop %c cities by weight:", GC.getCommerceInfo(COMMERCE_CULTURE).getChar()));
		int iLegendaryCulture = GC.getGameINLINE().getCultureThreshold((CultureLevelTypes)(GC.getNumCultureLevelInfos() - 1));
		std::vector<std::pair<int,int> > city_list; // (weight, city id)

		int iLoop;
		for (CvCity* pLoopCity = kPlayer.firstCity(&iLoop); pLoopCity != NULL; pLoopCity = kPlayer.nextCity(&iLoop))
			city_list.push_back(std::make_pair(kPlayer.AI_commerceWeight(COMMERCE_CULTURE, pLoopCity), pLoopCity->getID()));

		int iListCities = std::min((int)city_list.size(), 3);
		std::partial_sort(city_list.begin(), city_list.begin()+iListCities, city_list.end(), std::greater<std::pair<int,int> >());

		int iGoldCommercePercent = kPlayer.AI_estimateBreakEvenGoldPercent();

		for (int i = 0; i < iListCities; i++)
		{
			CvCity* pLoopCity = kPlayer.getCity(city_list[i].second);
			int iEstimatedRate = pLoopCity->getCommerceRate(COMMERCE_CULTURE);
			iEstimatedRate += (100 - iGoldCommercePercent - kPlayer.getCommercePercent(COMMERCE_CULTURE)) * pLoopCity->getYieldRate(YIELD_COMMERCE) * pLoopCity->getTotalCommerceRateModifier(COMMERCE_CULTURE) / 10000;
			int iCountdown = (iLegendaryCulture - pLoopCity->getCulture(kPlayer.getID())) / std::max(1, iEstimatedRate);

			szBuffer.append(CvWString::format(L"\n %s:\t%d%%, %d turns", pLoopCity->getName().GetCString(), city_list[i].first, iCountdown));
		}
		szBuffer.append(CvWString::format(L"\n(assuming %d%% gold)", iGoldCommercePercent));
	}
	
	// skip a line
	szBuffer.append(NEWLINE);
	szBuffer.append(NEWLINE);

	// show peace values
	bool bHadAny = false;
	bool bFirst = true;
	for (int iTeamIndex = 0; iTeamIndex < MAX_TEAMS; iTeamIndex++)
	{
		TeamTypes eLoopTeam = (TeamTypes) iTeamIndex;
		CvTeamAI& kLoopTeam = GET_TEAM(eLoopTeam);
		if (eLoopTeam != eTeam && kLoopTeam.isAlive() && !kLoopTeam.isBarbarian() && !kLoopTeam.isMinorCiv())
		{
			if (kTeam.isAtWar(eLoopTeam))
			{
				if (bFirst)
				{
					szBuffer.append(CvWString::format(SETCOLR L"Current War:\n" ENDCOLR, TEXT_COLOR("COLOR_UNIT_TEXT")));
					bFirst = false;
				}

				bHadAny = true;
				
				WarPlanTypes eWarPlan = kTeam.AI_getWarPlan(eLoopTeam);
				CvWStringBuffer szWarplan;
				GAMETEXT.getWarplanString(szWarplan, eWarPlan);
				// <advc.104>
				if(getWPAI.isEnabled()) {
					szBuffer.append(CvWString::format(
							SETCOLR L" %s (%d) with %s\n" ENDCOLR, 
							TEXT_COLOR("COLOR_NEGATIVE_TEXT"),
							szWarplan.getCString(),
							kTeam.AI_getWarPlanStateCounter(eLoopTeam),
							kLoopTeam.getName().GetCString()));
				}
				else { // </advc.104>
					int iOtherValue = kTeam.AI_endWarVal(eLoopTeam);
					int iTheirValue = kLoopTeam.AI_endWarVal(eTeam);
					szBuffer.append( CvWString::format(SETCOLR L" %s " ENDCOLR SETCOLR L"(%d, %d)" ENDCOLR SETCOLR L" with %s " ENDCOLR  SETCOLR L"(%d, %d)\n" ENDCOLR, 
						TEXT_COLOR((iOtherValue < iTheirValue) ? "COLOR_POSITIVE_TEXT" : "COLOR_NEGATIVE_TEXT"),
						szWarplan.getCString(),
						TEXT_COLOR((iOtherValue < iTheirValue) ? "COLOR_POSITIVE_TEXT" : "COLOR_NEGATIVE_TEXT"),
						iOtherValue, kTeam.AI_getWarSuccess(eLoopTeam),
						TEXT_COLOR((iOtherValue < iTheirValue) ? "COLOR_POSITIVE_TEXT" : "COLOR_NEGATIVE_TEXT"),
						kLoopTeam.getName().GetCString(),
						TEXT_COLOR((iTheirValue < iOtherValue) ? "COLOR_POSITIVE_TEXT" : "COLOR_NEGATIVE_TEXT"),
						iTheirValue, kLoopTeam.AI_getWarSuccess(eTeam)) );
				}
			}
		}
	}
	
	// double space if had any war
	if( kTeam.getAnyWarPlanCount(true) > 0 )
	{
		int iEnemyPowerPercent = kTeam.AI_getEnemyPowerPercent();
		szBuffer.append(CvWString::format(SETCOLR L"\nEnemy Power Percent: %d" ENDCOLR, TEXT_COLOR((iEnemyPowerPercent < 100) ? "COLOR_POSITIVE_TEXT" : "COLOR_NEGATIVE_TEXT"), iEnemyPowerPercent));
		
	}
	if (bHadAny)
	{
		int iWarSuccessRating = kTeam.AI_getWarSuccessRating();
		szBuffer.append(CvWString::format(SETCOLR L"\nWar Success Ratio: %d" ENDCOLR, TEXT_COLOR((iWarSuccessRating > 0) ? "COLOR_POSITIVE_TEXT" : "COLOR_NEGATIVE_TEXT"), iWarSuccessRating));
	}
	if (bHadAny || kTeam.getAnyWarPlanCount(true) > 0)
	{
		szBuffer.append(NEWLINE);
		szBuffer.append(NEWLINE);
	}
	
	// show warplan values
	bHadAny = false;
	bFirst = true;
	for (int iTeamIndex = 0; iTeamIndex < MAX_TEAMS; iTeamIndex++)
	{
		TeamTypes eLoopTeam = (TeamTypes) iTeamIndex;
		CvTeamAI& kLoopTeam = GET_TEAM(eLoopTeam);
		if (eLoopTeam != eTeam && kLoopTeam.isAlive() && !kLoopTeam.isBarbarian())
		{
			WarPlanTypes eWarPlan = kTeam.AI_getWarPlan(eLoopTeam);
			if (!kTeam.isAtWar(eLoopTeam) && eWarPlan != NO_WARPLAN)
			{
				if (bFirst)
				{
					szBuffer.append(CvWString::format(SETCOLR L"Imminent War:\n" ENDCOLR, TEXT_COLOR("COLOR_UNIT_TEXT")));
					bFirst = false;
				}

				bHadAny = true;

				CvWStringBuffer szWarplan;
				GAMETEXT.getWarplanString(szWarplan, eWarPlan);
				szBuffer.append(CvWString::format(SETCOLR L" %s (%d) with %s\n" ENDCOLR, TEXT_COLOR("COLOR_NEGATIVE_TEXT"),
					szWarplan.getCString(),
					// advc.104: Show war plan age instead of K-Mod's startWarVal
					getWPAI.isEnabled() ? kTeam.AI_getWarPlanStateCounter(eLoopTeam) :
					kTeam.AI_startWarVal(eLoopTeam, eWarPlan,
					true), // advc.001n
					kLoopTeam.getName().GetCString()));
			}
		}
	}
	
	// double space if had any war plan
	if (bHadAny)
		szBuffer.append(NEWLINE);

	// <advc.104> K-Mod/BBAI war percentages aren't helpful for testing UWAI
	if(getWPAI.isEnabled())
		return; // </advc.104>
	
	// calculate war percentages
	float fOverallWarPercentage = 0;
	bool bAggressive = GC.getGameINLINE().isOption(GAMEOPTION_AGGRESSIVE_AI);
	
	bool bAnyCapitalAreaAlone = kTeam.AI_isAnyCapitalAreaAlone();

	int iFinancialTroubleCount = 0;
	int iDaggerCount = 0;
	int iGetBetterUnitsCount = 0;
	for (int iI = 0; iI < MAX_PLAYERS; iI++)
	{
		if (GET_PLAYER((PlayerTypes)iI).isAlive())
		{
			if (GET_PLAYER((PlayerTypes)iI).getTeam() == eTeam)
			{
				if ( GET_PLAYER((PlayerTypes)iI).AI_isDoStrategy(AI_STRATEGY_DAGGER)
					|| GET_PLAYER((PlayerTypes)iI).AI_isDoVictoryStrategy(AI_VICTORY_CONQUEST3)
					|| GET_PLAYER((PlayerTypes)iI).AI_isDoVictoryStrategy(AI_VICTORY_DOMINATION4) )
				{
					iDaggerCount++;
					bAggressive = true;
				}

				if (GET_PLAYER((PlayerTypes)iI).AI_isDoStrategy(AI_STRATEGY_GET_BETTER_UNITS))
				{
					iGetBetterUnitsCount++;
				}
				
				if (GET_PLAYER((PlayerTypes)iI).AI_isFinancialTrouble())
				{
					iFinancialTroubleCount++;
				}
			}
		}
	}

	// calculate unit spending for use in iTotalWarRandThreshold
	int iNumMembers = kTeam.getNumMembers();

	// if random in this range is 0, we go to war of this type (so lower numbers are higher probablity)
	// average of everyone on our team
	int iTotalWarRand;
    int iLimitedWarRand;
    int iDogpileWarRand;
	kTeam.AI_getWarRands( iTotalWarRand, iLimitedWarRand, iDogpileWarRand );

	int iTotalWarThreshold;
	int iLimitedWarThreshold;
	int iDogpileWarThreshold;
	kTeam.AI_getWarThresholds( iTotalWarThreshold, iLimitedWarThreshold, iDogpileWarThreshold );

	// we oppose war if half the non-dagger teammates in financial trouble
	bool bFinancesOpposeWar = false;
	if ((iFinancialTroubleCount - iDaggerCount) >= std::max(1, kTeam.getNumMembers() / 2 ))
	{
		// this can be overridden by by the pro-war booleans
		bFinancesOpposeWar = true;
	}

	// if agressive, we may start a war to get money
	bool bFinancesProTotalWar = false;
	bool bFinancesProLimitedWar = false;
	bool bFinancesProDogpileWar = false;
	if (iFinancialTroubleCount > 0)
	{
		// do we like all out wars?
		if (iDaggerCount > 0 || iTotalWarRand < 100)
		{
			bFinancesProTotalWar = true;
		}

		// do we like limited wars?
		if (iLimitedWarRand < 100)
		{
			bFinancesProLimitedWar = true;
		}
		
		// do we like dogpile wars?
		if (iDogpileWarRand < 100)
		{
			bFinancesProDogpileWar = true;
		}
	}
	bool bFinancialProWar = (bFinancesProTotalWar || bFinancesProLimitedWar || bFinancesProDogpileWar);
	
	// overall war check (quite frequently true)
	if ((iGetBetterUnitsCount - iDaggerCount) * 3 < iNumMembers * 2)
	{
		if (bFinancialProWar || !bFinancesOpposeWar)
		{
			fOverallWarPercentage = (float)std::min(100, GC.getHandicapInfo(GC.getGameINLINE().getHandicapType()).getAIDeclareWarProb());
		}
	}

	// team power (if agressive, we use higher value)
	int iTeamPower = kTeam.getPower(true);
	if (bAggressive && kTeam.getAnyWarPlanCount(true) == 0)
	{
		iTeamPower *= 4;
		iTeamPower /= 3;
	}

	iTeamPower *= (100 - kTeam.AI_getEnemyPowerPercent());
	iTeamPower /= 100;
	
	// we will put the values into an array, then sort it for display
	int iBestPossibleMaxWarPass = MAX_INT;
	struct CvStartWarInfo
	{
		int		iStartWarValue;
		int		iNoWarAttitudeProb;
		int		iPossibleMaxWarPass;
		bool	bPossibleLimitedWar;
		bool	bPossibleDogpileWar;
		bool    bEnoughDogpilePower;

		bool	bValid;
		bool	bLandTarget;
		bool	bVictory4;
		//bool	bAnyCapitalAreaAlone; // advc.003: unused
		bool	bAdjacentCheckPassed;
		bool	bMaxWarNearbyPowerRatio;
		bool	bMaxWarDistantPowerRatio;
	} aStartWarInfo[MAX_TEAMS];
	
	// first calculate all the values and put into array
	for (int iTeamIndex = 0; iTeamIndex < MAX_TEAMS; iTeamIndex++)
	{
		aStartWarInfo[iTeamIndex].bValid = false;
		
		TeamTypes eLoopTeam = (TeamTypes) iTeamIndex;
		CvTeamAI& kLoopTeam = GET_TEAM(eLoopTeam);
		if (eLoopTeam != eTeam && kLoopTeam.isAlive() && !kLoopTeam.isBarbarian())
		{
			WarPlanTypes eWarPlan = kTeam.AI_getWarPlan(eLoopTeam);
			if (!kTeam.isAtWar(eLoopTeam) && (eWarPlan == NO_WARPLAN))
			{
				if (kTeam.canEventuallyDeclareWar(eLoopTeam) && kTeam.isHasMet(eLoopTeam))
				{
					if( GET_TEAM(eLoopTeam).isAVassal() && !kTeam.AI_isOkayVassalTarget(eLoopTeam) )
					{
						continue;
					}

					aStartWarInfo[iTeamIndex].bValid = true;
					
					int iLoopTeamPower = kLoopTeam.getDefensivePower();
					bool bLandTarget = kTeam.AI_isLandTarget(eLoopTeam);
					aStartWarInfo[iTeamIndex].bLandTarget = bLandTarget;

					bool bVictory4 = GET_TEAM(eLoopTeam).AI_isAnyMemberDoVictoryStrategyLevel4();
					aStartWarInfo[iTeamIndex].bVictory4 = bVictory4;

					//int iNoWarAttitudeProb = kTeam.AI_noWarAttitudeProb(kTeam.AI_getAttitude(eLoopTeam));
					int iNoWarAttitudeProb = std::max(kTeam.AI_noWarAttitudeProb(kTeam.AI_getAttitude(eLoopTeam)), kTeam.AI_noWarAttitudeProb(kTeam.AI_getAttitude(GET_TEAM(eLoopTeam).getMasterTeam())));
					aStartWarInfo[iTeamIndex].iNoWarAttitudeProb = iNoWarAttitudeProb;

					// total war
					aStartWarInfo[iTeamIndex].iPossibleMaxWarPass = MAX_INT;
					if (iNoWarAttitudeProb < 100 && (bFinancesProTotalWar || !bFinancesOpposeWar))
					{
						int iNoWarChance = range(iNoWarAttitudeProb - (bAggressive ? 10 : 0) - (bFinancesProTotalWar ? 10 : 0) + (20*iGetBetterUnitsCount)/iNumMembers, 0, 100);
						if (iNoWarChance < 100)
						{
							bool bMaxWarNearbyPowerRatio = (iLoopTeamPower < ((iTeamPower * kTeam.AI_maxWarNearbyPowerRatio()) / 100));
							bool bMaxWarDistantPowerRatio = (iLoopTeamPower < ((iTeamPower * kTeam.AI_maxWarDistantPowerRatio()) / 100));
							aStartWarInfo[iTeamIndex].bMaxWarNearbyPowerRatio = bMaxWarNearbyPowerRatio;
							aStartWarInfo[iTeamIndex].bMaxWarDistantPowerRatio = bMaxWarDistantPowerRatio;

							bool bAdjacentCheckPassed = true;
							int iMaxWarMinAdjacentPercent = kTeam.AI_maxWarMinAdjacentLandPercent();
							if (iMaxWarMinAdjacentPercent > 0)
							{
								int iMinAdjacentPlots = ((kTeam.getTotalLand() * iMaxWarMinAdjacentPercent) / 100);
								if (iMinAdjacentPlots > 0)
								{
									bAdjacentCheckPassed = (kTeam.AI_calculateAdjacentLandPlots(eLoopTeam) >= iMinAdjacentPlots);
								}
							}
							aStartWarInfo[iTeamIndex].bAdjacentCheckPassed = bAdjacentCheckPassed;

							// check to see which max war pass, if any is valid for this loop team
							int iPossibleMaxWarPass = MAX_INT;
							if (bMaxWarNearbyPowerRatio && (bAdjacentCheckPassed || bVictory4))
							{
								iPossibleMaxWarPass = 0;
							}
							else if (bMaxWarNearbyPowerRatio && (bLandTarget || bAnyCapitalAreaAlone || bVictory4))
							{
								iPossibleMaxWarPass = 1;
							}
							else if (bMaxWarDistantPowerRatio)
							{
								iPossibleMaxWarPass = 2;
							}
							aStartWarInfo[iTeamIndex].iPossibleMaxWarPass = iPossibleMaxWarPass;
							
							// if this team is valid on a lower pass, then it is the best pass
							if (iPossibleMaxWarPass < iBestPossibleMaxWarPass)
							{
								iBestPossibleMaxWarPass = iPossibleMaxWarPass;
							}
						}
					}

					// limited war
					aStartWarInfo[iTeamIndex].bPossibleLimitedWar = false;
					if (iNoWarAttitudeProb < 100 && (bFinancesProLimitedWar || !bFinancesOpposeWar))
					{
						int iNoWarChance = std::max(0, iNoWarAttitudeProb + 10 - (bAggressive ? 10 : 0) - (bFinancesProLimitedWar ? 10 : 0));
						if (iNoWarChance < 100)
						{
							bool bLimitedPowerRatio = (iLoopTeamPower < ((iTeamPower * kTeam.AI_limitedWarPowerRatio()) / 100));
							bool bAnyLoopTeamCapitalAreaAlone = kLoopTeam.AI_isAnyCapitalAreaAlone();
							
							if (bLimitedPowerRatio && (bLandTarget || (bAnyCapitalAreaAlone && bAnyLoopTeamCapitalAreaAlone)))
							{
								aStartWarInfo[iTeamIndex].bPossibleLimitedWar = true;
							}
						}
					}

					// dogpile war
					aStartWarInfo[iTeamIndex].bPossibleDogpileWar = false;
					aStartWarInfo[iTeamIndex].bEnoughDogpilePower = false;
					if (iNoWarAttitudeProb < 100 && (bFinancesProDogpileWar || !bFinancesOpposeWar) && kTeam.canDeclareWar(eLoopTeam))
					{
						int iNoWarChance = std::max(0, iNoWarAttitudeProb + 20 - (bAggressive ? 10 : 0) - (bFinancesProDogpileWar ? 10 : 0));
						if (iNoWarChance < 100)
						{
							int iDogpilePower = iTeamPower;
							for (int iTeamIndex2 = 0; iTeamIndex2 < MAX_CIV_TEAMS; iTeamIndex2++)
							{
								TeamTypes eDogpileLoopTeam = (TeamTypes) iTeamIndex2;
								CvTeamAI& kDogpileLoopTeam = GET_TEAM(eDogpileLoopTeam);
								if (kDogpileLoopTeam.isAlive())
								{
									if (eDogpileLoopTeam != eLoopTeam)
									{
										if (atWar(eDogpileLoopTeam, eLoopTeam))
										{
											iDogpilePower += kDogpileLoopTeam.getPower(false);
										}
									}
								}
							}

							bool bDogpilePowerRatio = (((iLoopTeamPower * 3) / 2) < iDogpilePower);
							aStartWarInfo[iTeamIndex].bPossibleDogpileWar = true;

							if (bDogpilePowerRatio)
							{
								aStartWarInfo[iTeamIndex].bEnoughDogpilePower = true;
							}
						}
					}
					
					// if this team can have any war, calculate the start war value
					aStartWarInfo[iTeamIndex].iStartWarValue = 0;
					if (aStartWarInfo[iTeamIndex].iPossibleMaxWarPass < MAX_INT || aStartWarInfo[iTeamIndex].bPossibleLimitedWar || aStartWarInfo[iTeamIndex].bPossibleDogpileWar)
					{
						aStartWarInfo[iTeamIndex].iStartWarValue = kTeam.AI_startWarVal(eLoopTeam, WARPLAN_TOTAL,
								true); // advc.001n
					}
				}
			}
		}
	}

	if( bFinancesOpposeWar )
	{
		szBuffer.append(CvWString::format(SETCOLR L"## Finances oppose war%s%s%s\n" ENDCOLR, TEXT_COLOR("COLOR_HIGHLIGHT_TEXT"),
			bFinancesProTotalWar ? L", pro Total" : L"",
			bFinancesProLimitedWar ? L", pro Limited" : L"",
			bFinancesProDogpileWar ? L", pro Dogpile" : L""));
		szBuffer.append(NEWLINE);
	}

	// display total war items, sorting the list
	bHadAny = false;
	bFirst = true;
	int iBestValue;
	int iLastValue = MAX_INT;
	do
	{
		// find the highest value item left to do
		iBestValue = 0;
		for (int iTeamIndex = 0; iTeamIndex < MAX_TEAMS; iTeamIndex++)
		{
			if (aStartWarInfo[iTeamIndex].bValid && aStartWarInfo[iTeamIndex].iPossibleMaxWarPass < MAX_INT)
			{
				if (aStartWarInfo[iTeamIndex].iStartWarValue > iBestValue && aStartWarInfo[iTeamIndex].iStartWarValue < iLastValue)
				{
					iBestValue = aStartWarInfo[iTeamIndex].iStartWarValue;
				}
			}
		}
		
		// did we find one?
		if (iBestValue > 0)
		{
			// setup for next loop
			iLastValue = iBestValue;

			// now display every team that has that value
			for (int iTeamIndex = 0; iTeamIndex < MAX_TEAMS; iTeamIndex++)
			{
				if (aStartWarInfo[iTeamIndex].bValid && aStartWarInfo[iTeamIndex].iStartWarValue == iBestValue)
				{
					CvTeamAI& kLoopTeam = GET_TEAM((TeamTypes) iTeamIndex);

					if (bFirst)
					{
						float fMaxWarPercentage = ((fOverallWarPercentage * (iTotalWarThreshold + 1)) / iTotalWarRand);
						szBuffer.append(CvWString::format(SETCOLR L"%.2f%% [%d/%d] Total War:\n" ENDCOLR, TEXT_COLOR("COLOR_UNIT_TEXT"), fMaxWarPercentage, (iTotalWarThreshold + 1), iTotalWarRand));
						bFirst = false;
					}

					bHadAny = true;
					
					int iNoWarChance = std::max(0, aStartWarInfo[iTeamIndex].iNoWarAttitudeProb - (bAggressive ? 10 : 0) - (bFinancesProTotalWar ? 10 : 0));
					int iTeamWarPercentage = (100 - iNoWarChance);
					
					if (aStartWarInfo[iTeamIndex].iPossibleMaxWarPass <= iBestPossibleMaxWarPass)
					{
						szBuffer.append(CvWString::format(SETCOLR L" %d%% %s%s war (%d) with %s\n" ENDCOLR, TEXT_COLOR("COLOR_ALT_HIGHLIGHT_TEXT"),
							iTeamWarPercentage,
							(aStartWarInfo[iTeamIndex].bVictory4) ? L"**" : L"",
							(aStartWarInfo[iTeamIndex].bLandTarget) ? L"land" : L"sea",
							aStartWarInfo[iTeamIndex].iStartWarValue, 
							kLoopTeam.getName().GetCString()));
					}
					else
					{
						szBuffer.append(CvWString::format(SETCOLR L" (%d%% %s%s war (%d) with %s [%s%s])\n" ENDCOLR, TEXT_COLOR("COLOR_HIGHLIGHT_TEXT"),
							iTeamWarPercentage,
							(aStartWarInfo[iTeamIndex].bVictory4) ? L"**" : L"",
							(aStartWarInfo[iTeamIndex].bLandTarget) ? L"land" : L"sea",
							aStartWarInfo[iTeamIndex].iStartWarValue, 
							kLoopTeam.getName().GetCString(),
							(iBestPossibleMaxWarPass == 0) ? ((aStartWarInfo[iTeamIndex].bMaxWarNearbyPowerRatio) ? L"not adjacent" : L"low power") : L"",
							(iBestPossibleMaxWarPass == 1) ? ((aStartWarInfo[iTeamIndex].bMaxWarNearbyPowerRatio) ? L"not land" : L"low power") : L""));
					}
				}
			}
		}
	}
	while (iBestValue > 0);

	// double space if had any war
	if (bHadAny)
	{
		szBuffer.append(NEWLINE);
	}
	
	// display limited war items, sorting the list
	bHadAny = false;
	bFirst = true;
	iLastValue = MAX_INT;
	do
	{
		// find the highest value item left to do
		iBestValue = 0;
		for (int iTeamIndex = 0; iTeamIndex < MAX_TEAMS; iTeamIndex++)
		{
			if (aStartWarInfo[iTeamIndex].bValid && aStartWarInfo[iTeamIndex].bPossibleLimitedWar)
			{
				if (aStartWarInfo[iTeamIndex].iStartWarValue > iBestValue && aStartWarInfo[iTeamIndex].iStartWarValue < iLastValue)
				{
					iBestValue = aStartWarInfo[iTeamIndex].iStartWarValue;
				}
			}
		}
		
		// did we find one?
		if (iBestValue > 0)
		{
			// setup for next loop
			iLastValue = iBestValue;

			// now display every team that has that value
			for (int iTeamIndex = 0; iTeamIndex < MAX_TEAMS; iTeamIndex++)
			{
				if (aStartWarInfo[iTeamIndex].bValid && aStartWarInfo[iTeamIndex].iStartWarValue == iBestValue)
				{
					if (bFirst)
					{
						float fLimitedWarPercentage = (fOverallWarPercentage * (iLimitedWarThreshold + 1)) / iLimitedWarRand;
						szBuffer.append(CvWString::format(SETCOLR L"%.2f%% Limited War:\n" ENDCOLR, TEXT_COLOR("COLOR_UNIT_TEXT"), fLimitedWarPercentage));
						bFirst = false;
					}

					bHadAny = true;
					
					int iNoWarChance = std::max(0, aStartWarInfo[iTeamIndex].iNoWarAttitudeProb + 10 - (bAggressive ? 10 : 0) - (bFinancesProLimitedWar ? 10 : 0));
					int iTeamWarPercentage = (100 - iNoWarChance);
					
					szBuffer.append(CvWString::format(SETCOLR L" %d%% %s%s war (%d) with %s\n" ENDCOLR, TEXT_COLOR("COLOR_ALT_HIGHLIGHT_TEXT"),
						iTeamWarPercentage,
						(aStartWarInfo[iTeamIndex].bVictory4) ? L"**" : L"",
						(aStartWarInfo[iTeamIndex].bLandTarget) ? L"land" : L"sea",
						aStartWarInfo[iTeamIndex].iStartWarValue, 
						GET_TEAM((TeamTypes) iTeamIndex).getName().GetCString()));
				}
			}
		}
	}
	while (iBestValue > 0);

	// double space if had any war
	if (bHadAny)
	{
		szBuffer.append(NEWLINE);
	}
	
	// display dogpile war items, sorting the list
	bHadAny = false;
	bFirst = true;
	iLastValue = MAX_INT;
	do
	{
		// find the highest value item left to do
		iBestValue = 0;
		for (int iTeamIndex = 0; iTeamIndex < MAX_TEAMS; iTeamIndex++)
		{
			if (aStartWarInfo[iTeamIndex].bValid && aStartWarInfo[iTeamIndex].bPossibleDogpileWar)
			{
				if (aStartWarInfo[iTeamIndex].iStartWarValue > iBestValue && aStartWarInfo[iTeamIndex].iStartWarValue < iLastValue)
				{
					iBestValue = aStartWarInfo[iTeamIndex].iStartWarValue;
				}
			}
		}
		
		// did we find one?
		if (iBestValue > 0)
		{
			// setup for next loop
			iLastValue = iBestValue;

			// now display every team that has that value
			for (int iTeamIndex = 0; iTeamIndex < MAX_TEAMS; iTeamIndex++)
			{
				if (aStartWarInfo[iTeamIndex].bValid && aStartWarInfo[iTeamIndex].iStartWarValue == iBestValue)
				{
					if (bFirst)
					{
						float fDogpileWarPercentage = (fOverallWarPercentage * (iDogpileWarThreshold + 1)) / iDogpileWarRand;
						szBuffer.append(CvWString::format(SETCOLR L"%.2f%% Dogpile War:\n" ENDCOLR, TEXT_COLOR("COLOR_UNIT_TEXT"), fDogpileWarPercentage));
						bFirst = false;
					}

					bHadAny = true;
					
					int iNoWarChance = std::max(0, aStartWarInfo[iTeamIndex].iNoWarAttitudeProb + 20 - (bAggressive ? 10 : 0) - (bFinancesProDogpileWar ? 10 : 0));
					int iTeamWarPercentage = (100 - iNoWarChance);
					
					if( aStartWarInfo[iTeamIndex].bEnoughDogpilePower )
					{
						if( (aStartWarInfo[iTeamIndex].bLandTarget) || (aStartWarInfo[iTeamIndex].bVictory4) )
						{
							szBuffer.append(CvWString::format(SETCOLR L" %d%% %s%s war (%d) with %s\n" ENDCOLR, TEXT_COLOR("COLOR_ALT_HIGHLIGHT_TEXT"),
								iTeamWarPercentage,
								(aStartWarInfo[iTeamIndex].bVictory4) ? L"**" : L"",
								L"land",
								aStartWarInfo[iTeamIndex].iStartWarValue, 
								GET_TEAM((TeamTypes) iTeamIndex).getName().GetCString()));
						}
						else
						{
							szBuffer.append(CvWString::format(SETCOLR L" %d%% %s%s war (%d) with %s\n" ENDCOLR, TEXT_COLOR("COLOR_HIGHLIGHT_TEXT"),
								iTeamWarPercentage,
								(aStartWarInfo[iTeamIndex].bVictory4) ? L"**" : L"",
								L"sea",
								aStartWarInfo[iTeamIndex].iStartWarValue, 
								GET_TEAM((TeamTypes) iTeamIndex).getName().GetCString()));
						}
					}
					else
					{
						szBuffer.append(CvWString::format(SETCOLR L" Lack power for %s%s war (%d) with %s\n" ENDCOLR, TEXT_COLOR("COLOR_HIGHLIGHT_TEXT"),
							(aStartWarInfo[iTeamIndex].bVictory4) ? L"**" : L"",
							(aStartWarInfo[iTeamIndex].bLandTarget) ? L"land" : L"sea",
							aStartWarInfo[iTeamIndex].iStartWarValue, 
							GET_TEAM((TeamTypes) iTeamIndex).getName().GetCString()));
					}
				}
			}
		}
	}
	while (iBestValue > 0);

	// double space if had any war
	if (bHadAny)
	{
		szBuffer.append(NEWLINE);
	}

	if (GC.shiftKey())
	{
		return;
	}
}
// K-Mod end

// advc.003j (comment): unused
void CvDLLWidgetData::parseConvertHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	if (widgetDataStruct.m_iData2 != 0)
	{
		if (widgetDataStruct.m_iData1 == NO_RELIGION)
		{
			szBuffer.assign(gDLL->getText("TXT_KEY_MISC_NO_STATE_REL"));
		}
		else
		{
			szBuffer.append(gDLL->getText("TXT_KEY_MISC_CONVERT_TO_REL", GC.getReligionInfo((ReligionTypes) widgetDataStruct.m_iData1).getTextKeyWide()));
		}
	}
	else
	{
		GAMETEXT.setConvertHelp(szBuffer, GC.getGameINLINE().getActivePlayer(), (ReligionTypes)widgetDataStruct.m_iData1);
	}
}

// advc.003j (comment): unused
void CvDLLWidgetData::parseRevolutionHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	if (widgetDataStruct.m_iData1 != 0)
	{
		szBuffer.assign(gDLL->getText("TXT_KEY_MISC_CHANGE_CIVICS"));
	}
	else
	{
		GAMETEXT.setRevolutionHelp(szBuffer, GC.getGameINLINE().getActivePlayer());
	}
}

/*void CvDLLWidgetData::parsePopupQueue(CvWidgetDataStruct &widgetDataStruct, CvWString &szBuffer) {
	PopupEventTypes eEvent;
	if ( m_pPopup || m_pScreen ) {
		if ( m_pPopup )
			eEvent = m_pPopup->getPopupType();
		else if ( m_pScreen )
			eEvent = m_pScreen->getPopupType();
		switch (eEvent) {
			case POPUPEVENT_NONE:
				szBuffer = gDLL->getText("TXT_KEY_MISC_NO_MOUSEOVER_TEXT");
				break;

			case POPUPEVENT_PRODUCTION:
				szBuffer = gDLL->getText("TXT_KEY_MISC_PRODUCTION_COMPLETE");
				break;

			case POPUPEVENT_TECHNOLOGY:
				szBuffer = gDLL->getText("TXT_KEY_MISC_TECH_RESEARCH_COMPLETE");
				break;

			case POPUPEVENT_RELIGION:
				szBuffer = gDLL->getText("TXT_KEY_MISC_NEW_REL_DISCOVERED");
				break;

			case POPUPEVENT_WARNING:
				szBuffer = gDLL->getText("TXT_KEY_MISC_WARNING");
				break;

			case POPUPEVENT_CIVIC:
				szBuffer = gDLL->getText("TXT_KEY_MISC_NEW_CIVIC_ACCESSIBLE");
				break;
		}
	}
}*/

void CvDLLWidgetData::parseAutomateCitizensHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	CvCity* pHeadSelectedCity;

	pHeadSelectedCity = gDLL->getInterfaceIFace()->getHeadSelectedCity();

	if (pHeadSelectedCity != NULL)
	{
		if (pHeadSelectedCity->isCitizensAutomated())
		{
			szBuffer.assign(gDLL->getText("TXT_KEY_MISC_OFF_CITIZEN_AUTO"));
		}
		else
		{
			szBuffer.assign(gDLL->getText("TXT_KEY_MISC_ON_CITIZEN_AUTO"));
		}
	}
}

void CvDLLWidgetData::parseAutomateProductionHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	CvCity* pHeadSelectedCity;

	pHeadSelectedCity = gDLL->getInterfaceIFace()->getHeadSelectedCity();

	if (pHeadSelectedCity != NULL)
	{
		if (pHeadSelectedCity->isProductionAutomated())
		{
			szBuffer.assign(gDLL->getText("TXT_KEY_MISC_OFF_PROD_AUTO"));
		}
		else
		{
			szBuffer.assign(gDLL->getText("TXT_KEY_MISC_ON_PROD_AUTO"));
		}
	}
}

void CvDLLWidgetData::parseEmphasizeHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	CvCity* pHeadSelectedCity;

	szBuffer.clear();

	pHeadSelectedCity = gDLL->getInterfaceIFace()->getHeadSelectedCity();

	if (pHeadSelectedCity != NULL)
	{
		if (pHeadSelectedCity->AI_isEmphasize((EmphasizeTypes)widgetDataStruct.m_iData1))
		{
			szBuffer.append(gDLL->getText("TXT_KEY_MISC_TURN_OFF"));
		}
		else
		{
			szBuffer.append(gDLL->getText("TXT_KEY_MISC_TURN_ON"));
		}
	}

	szBuffer.append(GC.getEmphasizeInfo((EmphasizeTypes)widgetDataStruct.m_iData1).getDescription());
}


void CvDLLWidgetData::parseTradeItem(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	CvWString szTempBuffer;
	szBuffer.clear();
	CvGame& g = GC.getGameINLINE();
	PlayerTypes eWhoFrom = NO_PLAYER;
	PlayerTypes eWhoTo = NO_PLAYER;
	if (widgetDataStruct.m_bOption)
	{
		if (gDLL->isDiplomacy())
		{
			eWhoFrom = (PlayerTypes)gDLL->getDiplomacyPlayer();
		}
		else if (gDLL->isMPDiplomacyScreenUp())
		{
			eWhoFrom = (PlayerTypes)gDLL->getMPDiplomacyPlayer();
		}
		eWhoTo = g.getActivePlayer();
	}
	else
	{
		eWhoFrom = g.getActivePlayer();
		if (gDLL->isDiplomacy())
		{
			eWhoTo = (PlayerTypes)gDLL->getDiplomacyPlayer();
		}
		else if (gDLL->isMPDiplomacyScreenUp())
		{
			eWhoTo = (PlayerTypes)gDLL->getMPDiplomacyPlayer();
		}
	}

	if (eWhoFrom == NO_PLAYER || eWhoTo == NO_PLAYER)
		return; // advc.003

	PlayerTypes eWhoDenies = eWhoFrom;
	TradeableItems eItemType = (TradeableItems)widgetDataStruct.m_iData1;
	switch (eItemType)
	{
	case TRADE_TECHNOLOGIES:
		GAMETEXT.setTechHelp(szBuffer, (TechTypes)widgetDataStruct.m_iData2);
		eWhoDenies = (widgetDataStruct.m_bOption ? eWhoFrom : eWhoTo);
		break;
	case TRADE_RESOURCES:
		GAMETEXT.setBonusHelp(szBuffer, (BonusTypes)widgetDataStruct.m_iData2);
		eWhoDenies = (widgetDataStruct.m_bOption ? eWhoFrom : eWhoTo);
		break;
	case TRADE_CITIES:
		szBuffer.assign(gDLL->getText("TXT_KEY_TRADE_CITIES"));
		eWhoDenies = (widgetDataStruct.m_bOption ? eWhoFrom : eWhoTo);
		break;
	case TRADE_PEACE:
		szBuffer.append(gDLL->getText("TXT_KEY_TRADE_MAKE_PEACE",
				TEAMREF(eWhoFrom).getName().GetCString(),
				GET_TEAM((TeamTypes)widgetDataStruct.m_iData2).getName().GetCString()));
		break;
	case TRADE_WAR:
		szBuffer.append(gDLL->getText("TXT_KEY_TRADE_MAKE_WAR",
				TEAMREF(eWhoFrom).getName().GetCString(),
				GET_TEAM((TeamTypes)widgetDataStruct.m_iData2).getName().GetCString()));
		break;
	case TRADE_EMBARGO:
		szBuffer.append(gDLL->getText("TXT_KEY_TRADE_STOP_TRADING",
				TEAMREF(eWhoFrom).getName().GetCString(),
				GET_TEAM((TeamTypes)widgetDataStruct.m_iData2).getName().GetCString()));
		break;
	case TRADE_CIVIC:
		szBuffer.append(gDLL->getText("TXT_KEY_TRADE_ADOPT_CIVIC",
				GC.getCivicInfo((CivicTypes)widgetDataStruct.m_iData2).
				getDescription()));
		break;
	case TRADE_RELIGION:
		szBuffer.append(gDLL->getText("TXT_KEY_TRADE_CONVERT_RELIGION",
				GC.getReligionInfo((ReligionTypes)widgetDataStruct.m_iData2).
				getDescription()));
		break;
	case TRADE_GOLD:
		szBuffer.append(gDLL->getText("TXT_KEY_TRADE_GOLD"));
		break;
	case TRADE_GOLD_PER_TURN:
		szBuffer.append(gDLL->getText("TXT_KEY_TRADE_GOLD_PER_TURN"));
		break;
	case TRADE_MAPS:
		szBuffer.append(gDLL->getText("TXT_KEY_TRADE_MAPS"));
		break;
	// BETTER_BTS_AI_MOD, Diplomacy, 12/07/09, jdog5000: START
	case TRADE_SURRENDER:
		szBuffer.append(gDLL->getText("TXT_KEY_TRADE_CAPITULATE"));
		eWhoDenies = (widgetDataStruct.m_bOption ? eWhoFrom : NO_PLAYER);
		break;
	case TRADE_VASSAL:
		szBuffer.append(gDLL->getText("TXT_KEY_TRADE_VASSAL"));
		eWhoDenies = (widgetDataStruct.m_bOption ? eWhoFrom : NO_PLAYER);
		break;
	// BETTER_BTS_AI_MOD: END
	case TRADE_OPEN_BORDERS:
		szBuffer.append(gDLL->getText("TXT_KEY_TRADE_OPEN_BORDERS"));
		break;
	case TRADE_DEFENSIVE_PACT:
		szBuffer.append(gDLL->getText("TXT_KEY_TRADE_DEFENSIVE_PACT"));
		break;
	case TRADE_PERMANENT_ALLIANCE:
		szBuffer.append(gDLL->getText("TXT_KEY_TRADE_PERMANENT_ALLIANCE"));
		break;
	case TRADE_PEACE_TREATY:
		szBuffer.append(gDLL->getText("TXT_KEY_TRADE_PEACE_TREATY",
				GC.getPEACE_TREATY_LENGTH()));
		break;
	// <advc.034>
	case TRADE_DISENGAGE:
		szBuffer.append(gDLL->getText("TXT_KEY_TRADE_DISENGAGE"));
		break; // </advc.034>
	}
	// <advc.072>
	if(CvDeal::isAnnual(eItemType) || eItemType == TRADE_PEACE_TREATY) {
		CvDeal* pDeal = g.nextCurrentDeal(eWhoFrom, eWhoTo, eItemType,
				widgetDataStruct.m_iData2, true);
		if(pDeal != NULL) {
			szBuffer.append(NEWLINE);
			szBuffer.append(NEWLINE);
			CvWString szReason;
			if(pDeal->isCancelable(g.getActivePlayer(), &szReason)) {
				szTempBuffer.Format(SETCOLR L"%s" ENDCOLR,
						TEXT_COLOR("COLOR_HIGHLIGHT_TEXT"),
						gDLL->getText("TXT_KEY_MISC_CLICK_TO_CANCEL").GetCString());
				szBuffer.append(szTempBuffer);
			}
			else szBuffer.append(szReason);
			szBuffer.append(L":");
			szBuffer.append(NEWLINE);
			if(eItemType == TRADE_PEACE_TREATY) // Don't want the duration in parentheses here
				szBuffer.append(gDLL->getText("TXT_KEY_TRADE_PEACE_TREATY_STR"));
			else GAMETEXT.getDealString(szBuffer, *pDeal, g.getActivePlayer());
			return; // No denial info
		}
	} // </advc.072>
	TradeData item;
	setTradeItem(&item, (TradeableItems)widgetDataStruct.m_iData1,
			widgetDataStruct.m_iData2);
	/*  advc.104l: Can't easily move this code elsewhere b/c the cache should
		only be used when TradeDenial is checked by this class. */
	WarEvaluator::enableCache();
	DenialTypes eDenial = GET_PLAYER(eWhoFrom).getTradeDenial(eWhoTo, item);
	WarEvaluator::disableCache(); // advc.104l
	if (eDenial == NO_DENIAL)
		return;

	// BETTER_BTS_AI_MOD, Diplomacy, 12/07/09, jdog5000: START
	if (eWhoDenies == NO_PLAYER)
	{
		switch(eDenial)
		{
		case DENIAL_POWER_US:
			eDenial = DENIAL_POWER_YOU;
			break;
		case DENIAL_POWER_YOU:
			eDenial = DENIAL_POWER_US;
			break;
		case DENIAL_WAR_NOT_POSSIBLE_US:
			eDenial = DENIAL_WAR_NOT_POSSIBLE_YOU;
			break;
		case DENIAL_WAR_NOT_POSSIBLE_YOU:
			eDenial = DENIAL_WAR_NOT_POSSIBLE_US;
			break;
		case DENIAL_PEACE_NOT_POSSIBLE_US:
			eDenial = DENIAL_PEACE_NOT_POSSIBLE_YOU;
			break;
		case DENIAL_PEACE_NOT_POSSIBLE_YOU:
			eDenial = DENIAL_PEACE_NOT_POSSIBLE_US;
			break;
		default :
			break;
		}
		szTempBuffer.Format(L"%s: " SETCOLR L"%s" ENDCOLR,
				GET_PLAYER(eWhoTo).getName(),
				TEXT_COLOR("COLOR_WARNING_TEXT"),
				GC.getDenialInfo(eDenial).getDescription());
	}
	else
	{
		szTempBuffer.Format(L"%s: " SETCOLR L"%s" ENDCOLR,
				GET_PLAYER(eWhoDenies).getName(),
				TEXT_COLOR("COLOR_WARNING_TEXT"),
				GC.getDenialInfo(eDenial).getDescription());
	}
	// BETTER_BTS_AI_MOD: END
	szBuffer.append(NEWLINE);
	szBuffer.append(szTempBuffer);
}


void CvDLLWidgetData::parseUnitModelHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	CvUnit* pHeadSelectedUnit = gDLL->getInterfaceIFace()->getHeadSelectedUnit();
	if (pHeadSelectedUnit != NULL)
		GAMETEXT.setUnitHelp(szBuffer, pHeadSelectedUnit);
}


void CvDLLWidgetData::parseFlagHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	CvWString szTempBuffer;
	// <advc.135c>
	CvGame const& g = GC.getGameINLINE();
	if(g.isNetworkMultiPlayer() && g.isDebugToolsAllowed(false)) {
		szTempBuffer.Format(SETCOLR L"%s" ENDCOLR, TEXT_COLOR("COLOR_WARNING_TEXT"),
				L"Cheats enabled");
		szBuffer.append(szTempBuffer);
		szBuffer.append(NEWLINE);
	} // </advc.135c>
	szTempBuffer.Format(SETCOLR L"%s" ENDCOLR, TEXT_COLOR("COLOR_HIGHLIGHT_TEXT"), GC.getCivilizationInfo(GC.getGameINLINE().getActiveCivilizationType()).getDescription());
	szBuffer.append(szTempBuffer);
	// <advc.700>
	if(g.isOption(GAMEOPTION_RISE_FALL)) {
		std::pair<int,int> rfCountdown = g.getRiseFall().getChapterCountdown();
		if(rfCountdown.second >= 0)
			szBuffer.append(L" (" + gDLL->getText("TXT_KEY_RF_CHAPTER_COUNTDOWN",
					rfCountdown.first, rfCountdown.second) + L")");
	} // </advc.700>
	szBuffer.append(NEWLINE);

	GAMETEXT.parseLeaderTraits(szBuffer, GET_PLAYER(GC.getGameINLINE().getActivePlayer()).getLeaderType(), GET_PLAYER(GC.getGameINLINE().getActivePlayer()).getCivilizationType());
}


void CvDLLWidgetData::parseMaintenanceHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	wchar szTempBuffer[1024];

	CvCity* pHeadSelectedCity = gDLL->getInterfaceIFace()->getHeadSelectedCity();

	if (pHeadSelectedCity == NULL)
		return; // advc.003

	if (pHeadSelectedCity->isWeLoveTheKingDay())
	{
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_MISC_WE_LOVE_KING_MAINT"));
		return; // advc.003
	}

	int iInflationFactor = 100 + GET_PLAYER(pHeadSelectedCity->getOwnerINLINE()).calculateInflationRate(); // K-Mod

	szBuffer.assign(gDLL->getText("TXT_KEY_MISC_MAINT_INFO"));
	szBuffer.append(NEWLINE);

	//iMaintenanceValue = pHeadSelectedCity->calculateDistanceMaintenanceTimes100();
	int iMaintenanceValue = pHeadSelectedCity->calculateDistanceMaintenanceTimes100()*iInflationFactor/100; // K-Mod
	if (iMaintenanceValue != 0)
	{
		CvWString szMaint = CvWString::format(L"%d.%02d", iMaintenanceValue/100, iMaintenanceValue%100);
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_MISC_NUM_MAINT_FLOAT", szMaint.GetCString()) + ((GET_PLAYER(pHeadSelectedCity->getOwnerINLINE()).getNumGovernmentCenters() > 0) ? gDLL->getText("TXT_KEY_MISC_DISTANCE_FROM_PALACE") : gDLL->getText("TXT_KEY_MISC_NO_PALACE_PENALTY")));
	}

	//iMaintenanceValue = pHeadSelectedCity->calculateNumCitiesMaintenanceTimes100();
	iMaintenanceValue = pHeadSelectedCity->calculateNumCitiesMaintenanceTimes100()*iInflationFactor/100; // K-Mod
	if (iMaintenanceValue != 0)
	{
		CvWString szMaint = CvWString::format(L"%d.%02d", iMaintenanceValue/100, iMaintenanceValue%100);
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_MISC_NUM_CITIES_FLOAT", szMaint.GetCString()));
	}

	//iMaintenanceValue = pHeadSelectedCity->calculateColonyMaintenanceTimes100();
	iMaintenanceValue = pHeadSelectedCity->calculateColonyMaintenanceTimes100()*iInflationFactor/100; // K-Mod
	if (iMaintenanceValue != 0)
	{
		CvWString szMaint = CvWString::format(L"%d.%02d", iMaintenanceValue/100, iMaintenanceValue%100);
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_MISC_COLONY_MAINT_FLOAT", szMaint.GetCString()));
	}

	//iMaintenanceValue = pHeadSelectedCity->calculateCorporationMaintenanceTimes100();
	iMaintenanceValue = pHeadSelectedCity->calculateCorporationMaintenanceTimes100()*iInflationFactor/100; // K-Mod
	if (iMaintenanceValue != 0)
	{
		CvWString szMaint = CvWString::format(L"%d.%02d", iMaintenanceValue/100, iMaintenanceValue%100);
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_MISC_CORPORATION_MAINT_FLOAT", szMaint.GetCString()));
	}

	szBuffer.append(SEPARATOR);

	/* CvWString szMaint = CvWString::format(L"%d.%02d", pHeadSelectedCity->getMaintenanceTimes100()/100, pHeadSelectedCity->getMaintenanceTimes100()%100);
	szBuffer.append(NEWLINE);
	szBuffer.append(gDLL->getText("TXT_KEY_MISC_TOTAL_MAINT_FLOAT", szMaint.GetCString())); */
	// K-Mod
	iMaintenanceValue = pHeadSelectedCity->getMaintenanceTimes100()*iInflationFactor/100;

	CvWString szMaint = CvWString::format(L"%d.%02d", iMaintenanceValue/100, iMaintenanceValue%100);
	szBuffer.append(NEWLINE);
	szBuffer.append(gDLL->getText("TXT_KEY_MISC_TOTAL_MAINT_FLOAT", szMaint.GetCString()));
	//

	iMaintenanceValue = pHeadSelectedCity->getMaintenanceModifier();

	if (iMaintenanceValue != 0)
	{
		swprintf(szTempBuffer, L" (%s%d%%)", ((iMaintenanceValue > 0) ? L"+" : L""), iMaintenanceValue);
		szBuffer.append(szTempBuffer);
	}

// BUG - Building Saved Maintenance - start
	if (pHeadSelectedCity->getOwnerINLINE() == GC.getGameINLINE().getActivePlayer() &&
			(getBugOptionBOOL("MiscHover__BuildingSavedMaintenance", false) ||
			GC.altKey())) // advc.063
		GAMETEXT.setBuildingSavedMaintenanceHelp(szBuffer, *pHeadSelectedCity, DOUBLE_SEPARATOR);
// BUG - Building Saved Maintenance - end
}


void CvDLLWidgetData::parseHealthHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	CvCity* pHeadSelectedCity = gDLL->getInterfaceIFace()->getHeadSelectedCity();
	if (pHeadSelectedCity == NULL)
		return; // advc.003

	GAMETEXT.setBadHealthHelp(szBuffer, *pHeadSelectedCity);
	szBuffer.append(L"\n=======================\n");
	GAMETEXT.setGoodHealthHelp(szBuffer, *pHeadSelectedCity);

// BUG - Building Additional Health - start
	if (pHeadSelectedCity->getOwnerINLINE() == GC.getGameINLINE().getActivePlayer() &&
			(getBugOptionBOOL("MiscHover__BuildingAdditionalHealth", false)
			|| GC.altKey())) // advc.063
		GAMETEXT.setBuildingAdditionalHealthHelp(szBuffer, *pHeadSelectedCity, DOUBLE_SEPARATOR);
// BUG - Building Additional Health - end

}


void CvDLLWidgetData::parseNationalityHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)  // advc.003: style changes
{
	wchar szTempBuffer[1024];

	szBuffer.assign(gDLL->getText("TXT_KEY_MISC_CITY_NATIONALITY"));
	CvCity* pHeadSelectedCity = gDLL->getInterfaceIFace()->getHeadSelectedCity();
	if(pHeadSelectedCity == NULL)
		return;
	CvCity const& c = *pHeadSelectedCity;
	/*  <advc.099> Replaced "Alive" with "EverAlive", and sorted to match the
		order in updateCityScreen (CvMainInterface.py) */
	std::vector<std::pair<int,PlayerTypes> > sorted;
	for(int i = 0; i < MAX_PLAYERS; i++) {
		CvPlayer const& kPlayer = GET_PLAYER((PlayerTypes)i);
		if(!kPlayer.isEverAlive())
			continue;
		int iCulturePercent = c.plot()->calculateCulturePercent(kPlayer.getID());
		if(iCulturePercent > 0)
			sorted.push_back(std::pair<int,PlayerTypes>(iCulturePercent, kPlayer.getID()));
	}
	std::sort(sorted.begin(), sorted.end());
	std::reverse(sorted.begin(), sorted.end());
	for(size_t i = 0; i < sorted.size(); i++) {
		CvPlayer const& kPlayer = GET_PLAYER(sorted[i].second);
		int iCulturePercent = sorted[i].first; // </advc.099>
		swprintf(szTempBuffer, L"\n%d%% " SETCOLR L"%s" ENDCOLR,
				iCulturePercent, PLAYER_TEXT_COLOR(kPlayer), kPlayer.getCivilizationAdjective());
		szBuffer.append(szTempBuffer);
	}
	PlayerTypes eCulturalOwner = c.calculateCulturalOwner(); // advc.099c
	// <advc.101> <advc.023>
	double prDecr = c.probabilityOccupationDecrement();
	double prDisplay = c.revoltProbability(true, false, true);
	if(prDisplay > 0) {
		double truePr = c.revoltProbability() * (1 - prDecr);
		swprintf(szTempBuffer, (truePr == 0 ? L"%.0f" : L"%.1f"),
				(float)(100 * truePr)); // </advc.023></advc.101>
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_MISC_CHANCE_OF_REVOLT", szTempBuffer));
		// <advc.023> Probability after occupation
		if(truePr != prDisplay) {
			szBuffer.append(NEWLINE);
			swprintf(szTempBuffer, L"%.1f", (float)(100 * prDisplay));
			if(eCulturalOwner == BARBARIAN_PLAYER || !GET_PLAYER(eCulturalOwner).isAlive())
				szBuffer.append(gDLL->getText("TXT_KEY_NO_BARB_REVOLT_IN_OCCUPATION",
						szTempBuffer));
			else szBuffer.append(gDLL->getText("TXT_KEY_NO_REVOLT_IN_OCCUPATION",
						szTempBuffer));
		} // </advc.023>
		// <advc.101> Warn about flipping
		swprintf(szTempBuffer, L"%d", c.getNumRevolts(eCulturalOwner));
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_MISC_PRIOR_REVOLTS", szTempBuffer));
		if(truePr > 0 && c.canCultureFlip(eCulturalOwner)) {
			szBuffer.append(NEWLINE);
			szBuffer.append(gDLL->getText("TXT_KEY_MISC_FLIP_WARNING", szTempBuffer));
		}
	}
	if(c.isOccupation()) { // </advc.101>
		// <advc.023>
		// Show probability of decrementing occupation - unless it's always 1
		if(GC.getDefineINT("OCCUPATION_COUNTDOWN_EXPONENT") > 0) {
			szBuffer.append(NEWLINE);
			szBuffer.append(gDLL->getText("TXT_KEY_OCCUPATION_TIMER",
					c.getOccupationTimer()));
			szBuffer.append(NEWLINE);
			// Tends to be a high-ish probability, no need for decimal places
			int prDecrPercent = ::round(prDecr * 100);
			// But don't claim it's 0% or 100% if it isn't quite
			if(prDecr > 0) prDecrPercent = std::max(1, prDecrPercent);
			if(prDecr < 1) prDecrPercent = std::min(99, prDecrPercent);
			szBuffer.append(gDLL->getText("TXT_KEY_TIMER_DECREASE_CHANCE",
					prDecrPercent));
		}
	} // </advc.023>
}

void CvDLLWidgetData::parseHappinessHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	CvCity* pHeadSelectedCity = gDLL->getInterfaceIFace()->getHeadSelectedCity();
	if (pHeadSelectedCity == NULL)
		return; // advc.003

	GAMETEXT.setAngerHelp(szBuffer, *pHeadSelectedCity);
	szBuffer.append(L"\n=======================\n");
	GAMETEXT.setHappyHelp(szBuffer, *pHeadSelectedCity);

// BUG - Building Additional Happiness - start
	if (pHeadSelectedCity->getOwnerINLINE() == GC.getGameINLINE().getActivePlayer() &&
			(getBugOptionBOOL("MiscHover__BuildingAdditionalHappiness", false)
			|| GC.altKey())) // advc.063
		GAMETEXT.setBuildingAdditionalHappinessHelp(szBuffer, *pHeadSelectedCity, DOUBLE_SEPARATOR);
// BUG - Building Additional Happiness - end
}


void CvDLLWidgetData::parsePopulationHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	CvCity* pHeadSelectedCity = gDLL->getInterfaceIFace()->getHeadSelectedCity();
	if (pHeadSelectedCity != NULL)
	{
		szBuffer.assign(gDLL->getText("TXT_KEY_MISC_FOOD_THRESHOLD",
				pHeadSelectedCity->getFood(), pHeadSelectedCity->growthThreshold()));
	}
}


void CvDLLWidgetData::parseProductionHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	CvCity* pHeadSelectedCity = gDLL->getInterfaceIFace()->getHeadSelectedCity();
	if (pHeadSelectedCity != NULL &&
		pHeadSelectedCity->getProductionNeeded() != MAX_INT) {
		CvWString szTemp;
		szTemp.Format(L"%s: %d/%d %c", pHeadSelectedCity->getProductionName(),
				pHeadSelectedCity->getProduction(),
				pHeadSelectedCity->getProductionNeeded(),
				GC.getYieldInfo(YIELD_PRODUCTION).getChar());
		szBuffer.assign(szTemp);
		/*  advc.064 (comment): Would be nice to show some hurry info here,
			if only to explain what the hurry-related icons on the production bar
			mean. However, untangling parseHurryHelp to avoid code duplication
			is too much work, so ... */
	}
}


void CvDLLWidgetData::parseCultureHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	CvCity* pHeadSelectedCity = gDLL->getInterfaceIFace()->getHeadSelectedCity();
	if (pHeadSelectedCity == NULL)
		return; // advc.003

	int iCultureTimes100 = pHeadSelectedCity->getCultureTimes100(pHeadSelectedCity->getOwnerINLINE());
	if (iCultureTimes100%100 == 0)
	{
		szBuffer.assign(gDLL->getText("TXT_KEY_MISC_CULTURE", iCultureTimes100/100, pHeadSelectedCity->getCultureThreshold()));
	}
	else
	{
		CvWString szCulture = CvWString::format(L"%d.%02d", iCultureTimes100/100, iCultureTimes100%100);
		szBuffer.assign(gDLL->getText("TXT_KEY_MISC_CULTURE_FLOAT", szCulture.GetCString(), pHeadSelectedCity->getCultureThreshold()));
	}
	// <advc.042> Code moved into subroutine
	int iTurnsLeft = pHeadSelectedCity->getCultureTurnsLeft();
	if(iTurnsLeft > 0) {
		szBuffer.append(L' ');
		szBuffer.append(gDLL->getText("INTERFACE_CITY_TURNS", std::max(1, iTurnsLeft)));
	} // </advc.042>

	szBuffer.append(L"\n=======================\n");
	GAMETEXT.setCommerceHelp(szBuffer, *pHeadSelectedCity, COMMERCE_CULTURE);
}


void CvDLLWidgetData::parseGreatPeopleHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	CvCity* pHeadSelectedCity = gDLL->getInterfaceIFace()->getHeadSelectedCity();
	if (pHeadSelectedCity != NULL)
		GAMETEXT.parseGreatPeopleHelp(szBuffer, *pHeadSelectedCity);
}


void CvDLLWidgetData::parseGreatGeneralHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	if (GC.getGameINLINE().getActivePlayer() != NO_PLAYER)
		GAMETEXT.parseGreatGeneralHelp(szBuffer, GET_PLAYER(GC.getGameINLINE().getActivePlayer()));
}


void CvDLLWidgetData::parseSelectedHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)  // advc.003: style changes
{
	//CvUnit* pHeadSelectedUnit = gDLL->getInterfaceIFace()->getHeadSelectedUnit(); // advc.003: unused
	CvCity* pHeadSelectedCity = gDLL->getInterfaceIFace()->getHeadSelectedCity();
	if (pHeadSelectedCity == NULL)
		return;

	OrderData* pOrder = pHeadSelectedCity->getOrderFromQueue(widgetDataStruct.m_iData1);
	if (pOrder == NULL)
		return;

	switch (pOrder->eOrderType)
	{
	case ORDER_TRAIN:
		GAMETEXT.setUnitHelp(szBuffer, ((UnitTypes)(pOrder->iData1)), false, false, false, pHeadSelectedCity);
		break;

	case ORDER_CONSTRUCT:
		//GAMETEXT.setBuildingHelp(szBuffer, (BuildingTypes)pOrder->iData1), false, false, false, pHeadSelectedCity);
// BUG - Building Actual Effects - start
		GAMETEXT.setBuildingHelpActual(szBuffer, (BuildingTypes)pOrder->iData1, false, false, false, pHeadSelectedCity);
// BUG - Building Actual Effects - end
		break;

	case ORDER_CREATE:
		GAMETEXT.setProjectHelp(szBuffer, (ProjectTypes)pOrder->iData1, false, pHeadSelectedCity);
		break;

	case ORDER_MAINTAIN:
		GAMETEXT.setProcessHelp(szBuffer, (ProcessTypes)pOrder->iData1);
		break;

	default:
		FAssertMsg(false, "eOrderType did not match valid options");
		break;
	}
}


void CvDLLWidgetData::parseTradeRouteCityHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	if (widgetDataStruct.m_iData2 != 0)
	{
		GAMETEXT.setTradeRouteHelp(szBuffer, widgetDataStruct.m_iData1, gDLL->getInterfaceIFace()->getHeadSelectedCity());
	}
}

void CvDLLWidgetData::parseEspionageCostHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	CvUnit* pUnit = gDLL->getInterfaceIFace()->getHeadSelectedUnit();
	if (NULL != pUnit)
	{
		CvPlot* pPlot = pUnit->plot();
		if (NULL != pPlot)
		{
			GAMETEXT.setEspionageCostHelp(szBuffer, (EspionageMissionTypes)widgetDataStruct.m_iData1, pPlot->getOwnerINLINE(), pPlot, widgetDataStruct.m_iData2, pUnit);
		}
	}
}

void CvDLLWidgetData::parseBuildingHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	if (widgetDataStruct.m_iData2 != 0)
	{
		GAMETEXT.setBuildingHelp(szBuffer, ((BuildingTypes)(widgetDataStruct.m_iData1)), false, false, widgetDataStruct.m_bOption, gDLL->getInterfaceIFace()->getHeadSelectedCity());
	}
}

void CvDLLWidgetData::parseProjectHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	if (widgetDataStruct.m_iData2 != 0)
	{
		GAMETEXT.setProjectHelp(szBuffer, ((ProjectTypes)widgetDataStruct.m_iData1), false, gDLL->getInterfaceIFace()->getHeadSelectedCity());
	}
}


void CvDLLWidgetData::parseTerrainHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	if (widgetDataStruct.m_iData2 != 0)
	{
		GAMETEXT.setTerrainHelp(szBuffer, (TerrainTypes)widgetDataStruct.m_iData1);
	}
}


void CvDLLWidgetData::parseFeatureHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	if (widgetDataStruct.m_iData2 != 0)
	{
		GAMETEXT.setFeatureHelp(szBuffer, (FeatureTypes)widgetDataStruct.m_iData1);
	}
}


void CvDLLWidgetData::parseTechEntryHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{	/*  advc.085: 0 was unused, so this check wasn't needed for anything. Now 0
		causes the scoreboard to expand (handled by caller). */
	//if (widgetDataStruct.m_iData2 != 0)
	GAMETEXT.setTechHelp(szBuffer, ((TechTypes)(widgetDataStruct.m_iData1)));
}

// BULL - Trade Denial - start
void CvDLLWidgetData::parseTechTradeEntryHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	if (widgetDataStruct.m_iData2 == -1)
		parseTechEntryHelp(widgetDataStruct, szBuffer);
	else {
		GAMETEXT.setTechTradeHelp(szBuffer, (TechTypes)widgetDataStruct.m_iData1,
				(PlayerTypes)widgetDataStruct.m_iData2);
	}
}
// BULL - Trade Denial - end

void CvDLLWidgetData::parseTechPrereqHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
//	szBuffer.Format(L"%cThis technology requires the knowledge of %s", gDLL->getSymbolID(BULLET_CHAR), GC.getTechInfo((TechTypes) widgetDataStruct.m_iData1).getDescription());
	szBuffer.assign(gDLL->getText("TXT_KEY_MISC_TECH_REQUIRES_KNOWLEDGE_OF", GC.getTechInfo((TechTypes) widgetDataStruct.m_iData1).getTextKeyWide()));
}

void CvDLLWidgetData::parseTechTreePrereq(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer, bool bTreeInfo)
{
	GAMETEXT.setTechHelp(szBuffer, (TechTypes)widgetDataStruct.m_iData1, false, false, false, bTreeInfo, (TechTypes)widgetDataStruct.m_iData2);
}


void CvDLLWidgetData::parseObsoleteHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	GAMETEXT.buildObsoleteString(szBuffer, ((TechTypes)(widgetDataStruct.m_iData1)));
}

void CvDLLWidgetData::parseObsoleteBonusString(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	GAMETEXT.buildObsoleteBonusString(szBuffer, ((BonusTypes)(widgetDataStruct.m_iData1)));
}

void CvDLLWidgetData::parseObsoleteSpecialHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	GAMETEXT.buildObsoleteSpecialString(szBuffer, ((TechTypes)(widgetDataStruct.m_iData1)));
}

void CvDLLWidgetData::parseMoveHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	GAMETEXT.buildMoveString(szBuffer, ((TechTypes)(widgetDataStruct.m_iData1)));
}

void CvDLLWidgetData::parseFreeUnitHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	GAMETEXT.buildFreeUnitString(szBuffer, ((TechTypes)(widgetDataStruct.m_iData2)));
}

void CvDLLWidgetData::parseFeatureProductionHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	GAMETEXT.buildFeatureProductionString(szBuffer, ((TechTypes)(widgetDataStruct.m_iData1)));
}

void CvDLLWidgetData::parseWorkerRateHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	GAMETEXT.buildWorkerRateString(szBuffer, ((TechTypes)(widgetDataStruct.m_iData1)));
}

void CvDLLWidgetData::parseTradeRouteHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	GAMETEXT.buildTradeRouteString(szBuffer, ((TechTypes)(widgetDataStruct.m_iData1)));
}

void CvDLLWidgetData::parseHealthRateHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	GAMETEXT.buildHealthRateString(szBuffer, ((TechTypes)(widgetDataStruct.m_iData1)));
}

void CvDLLWidgetData::parseHappinessRateHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	GAMETEXT.buildHappinessRateString(szBuffer, ((TechTypes)(widgetDataStruct.m_iData1)));
}

void CvDLLWidgetData::parseFreeTechHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	GAMETEXT.buildFreeTechString(szBuffer, ((TechTypes)(widgetDataStruct.m_iData1)));
}

void CvDLLWidgetData::parseLOSHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	GAMETEXT.buildLOSString(szBuffer, ((TechTypes)(widgetDataStruct.m_iData1)));
}

void CvDLLWidgetData::parseMapCenterHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	GAMETEXT.buildMapCenterString(szBuffer, ((TechTypes)(widgetDataStruct.m_iData1)));
}

void CvDLLWidgetData::parseMapRevealHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	GAMETEXT.buildMapRevealString(szBuffer, ((TechTypes)(widgetDataStruct.m_iData1)));
}

void CvDLLWidgetData::parseMapTradeHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	GAMETEXT.buildMapTradeString(szBuffer, ((TechTypes)(widgetDataStruct.m_iData1)));
}

void CvDLLWidgetData::parseTechTradeHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	GAMETEXT.buildTechTradeString(szBuffer, ((TechTypes)(widgetDataStruct.m_iData1)));
}

void CvDLLWidgetData::parseGoldTradeHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	GAMETEXT.buildGoldTradeString(szBuffer, ((TechTypes)(widgetDataStruct.m_iData1)));
}

void CvDLLWidgetData::parseOpenBordersHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	GAMETEXT.buildOpenBordersString(szBuffer, ((TechTypes)(widgetDataStruct.m_iData1)));
}

void CvDLLWidgetData::parseDefensivePactHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	GAMETEXT.buildDefensivePactString(szBuffer, ((TechTypes)(widgetDataStruct.m_iData1)));
}

void CvDLLWidgetData::parsePermanentAllianceHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	GAMETEXT.buildPermanentAllianceString(szBuffer, ((TechTypes)(widgetDataStruct.m_iData1)));
}

void CvDLLWidgetData::parseVassalStateHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	GAMETEXT.buildVassalStateString(szBuffer, ((TechTypes)(widgetDataStruct.m_iData1)));
}

void CvDLLWidgetData::parseBuildBridgeHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	GAMETEXT.buildBridgeString(szBuffer, ((TechTypes)(widgetDataStruct.m_iData1)));
}

void CvDLLWidgetData::parseIrrigationHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	GAMETEXT.buildIrrigationString(szBuffer, ((TechTypes)(widgetDataStruct.m_iData1)));
}

void CvDLLWidgetData::parseIgnoreIrrigationHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	GAMETEXT.buildIgnoreIrrigationString(szBuffer, ((TechTypes)(widgetDataStruct.m_iData1)));
}

void CvDLLWidgetData::parseWaterWorkHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	GAMETEXT.buildWaterWorkString(szBuffer, ((TechTypes)(widgetDataStruct.m_iData1)));
}

void CvDLLWidgetData::parseBuildHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	GAMETEXT.buildImprovementString(szBuffer, ((TechTypes)(widgetDataStruct.m_iData1)), widgetDataStruct.m_iData2);
}

void CvDLLWidgetData::parseDomainExtraMovesHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	GAMETEXT.buildDomainExtraMovesString(szBuffer, ((TechTypes)(widgetDataStruct.m_iData1)), widgetDataStruct.m_iData2);
}

void CvDLLWidgetData::parseAdjustHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	GAMETEXT.buildAdjustString(szBuffer, ((TechTypes)(widgetDataStruct.m_iData1)), widgetDataStruct.m_iData2);
}

void CvDLLWidgetData::parseTerrainTradeHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	if (widgetDataStruct.m_iData2 < GC.getNumTerrainInfos())
	{
		GAMETEXT.buildTerrainTradeString(szBuffer, ((TechTypes)(widgetDataStruct.m_iData1)), widgetDataStruct.m_iData2);
	}
	else if (widgetDataStruct.m_iData2 == GC.getNumTerrainInfos())
	{
		GAMETEXT.buildRiverTradeString(szBuffer, ((TechTypes)(widgetDataStruct.m_iData1)));
	}
}

void CvDLLWidgetData::parseSpecialBuildingHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	GAMETEXT.buildSpecialBuildingString(szBuffer, ((TechTypes)(widgetDataStruct.m_iData1)), widgetDataStruct.m_iData2);
}

void CvDLLWidgetData::parseYieldChangeHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	GAMETEXT.buildYieldChangeString(szBuffer, ((TechTypes)(widgetDataStruct.m_iData1)), widgetDataStruct.m_iData2, false);
}

void CvDLLWidgetData::parseBonusRevealHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	GAMETEXT.buildBonusRevealString(szBuffer, ((TechTypes)(widgetDataStruct.m_iData1)), widgetDataStruct.m_iData2, true);
}

void CvDLLWidgetData::parseCivicRevealHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	GAMETEXT.buildCivicRevealString(szBuffer, ((TechTypes)(widgetDataStruct.m_iData1)), widgetDataStruct.m_iData2, true);
}

void CvDLLWidgetData::parseProcessInfoHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	GAMETEXT.buildProcessInfoString(szBuffer, ((TechTypes)(widgetDataStruct.m_iData1)), widgetDataStruct.m_iData2, true);
}

void CvDLLWidgetData::parseFoundReligionHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	GAMETEXT.buildFoundReligionString(szBuffer, ((TechTypes)(widgetDataStruct.m_iData1)), widgetDataStruct.m_iData2, true);
}

void CvDLLWidgetData::parseFoundCorporationHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	GAMETEXT.buildFoundCorporationString(szBuffer, ((TechTypes)(widgetDataStruct.m_iData1)), widgetDataStruct.m_iData2, true);
}

void CvDLLWidgetData::parseFinanceNumUnits(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{	// advc.086: Disabled (noninformative)
	//szBuffer.assign(gDLL->getText("TXT_KEY_ECON_NUM_UNITS_SUPPORTING"));
}

void CvDLLWidgetData::parseFinanceUnitCost(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{	// advc.086: Disabled
	//szBuffer.assign(gDLL->getText("TXT_KEY_ECON_MONEY_SPENT_UPKEEP"));
	if (widgetDataStruct.m_iData2 > 0)
		GAMETEXT.buildFinanceUnitCostString(szBuffer, (PlayerTypes)widgetDataStruct.m_iData1);
}

void CvDLLWidgetData::parseFinanceAwaySupply(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	szBuffer.assign(gDLL->getText("TXT_KEY_ECON_AMOUNT_MONEY_UNITS_ENEMY_TERRITORY"));
	if (widgetDataStruct.m_iData2 > 0)
		GAMETEXT.buildFinanceAwaySupplyString(szBuffer, (PlayerTypes)widgetDataStruct.m_iData1);
}

void CvDLLWidgetData::parseFinanceCityMaint(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{	// advc.086: Disabled
	//szBuffer.assign(gDLL->getText("TXT_KEY_ECON_AMOUNT_MONEY_CITY_MAINT"));
	if (widgetDataStruct.m_iData2 > 0)
		GAMETEXT.buildFinanceCityMaintString(szBuffer, (PlayerTypes)widgetDataStruct.m_iData1);
}

void CvDLLWidgetData::parseFinanceCivicUpkeep(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{	// advc.086: Disabled
	//szBuffer.assign(gDLL->getText("TXT_KEY_ECON_AMOUNT_MONEY_CIVIC_UPKEEP"));
	if (widgetDataStruct.m_iData2 > 0)
		GAMETEXT.buildFinanceCivicUpkeepString(szBuffer, (PlayerTypes)widgetDataStruct.m_iData1);
}

void CvDLLWidgetData::parseFinanceForeignIncome(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	szBuffer.assign(gDLL->getText("TXT_KEY_ECON_AMOUNT_MONEY_FOREIGN"));
	if (widgetDataStruct.m_iData2 > 0)
		GAMETEXT.buildFinanceForeignIncomeString(szBuffer, (PlayerTypes)widgetDataStruct.m_iData1);
}

void CvDLLWidgetData::parseFinanceInflatedCosts(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{	// advc (comment): Unused b/c of K-Mod changes to EconomicsAdvisor.py
	szBuffer.assign(gDLL->getText("TXT_KEY_ECON_AMOUNT_MONEY_AFTER_INFLATION"));
	if (widgetDataStruct.m_iData2 > 0)
		GAMETEXT.buildFinanceInflationString(szBuffer, (PlayerTypes)widgetDataStruct.m_iData1);
}

void CvDLLWidgetData::parseFinanceGrossIncome(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	szBuffer.assign(gDLL->getText("TXT_KEY_ECON_GROSS_INCOME"));
}

void CvDLLWidgetData::parseFinanceNetGold(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	szBuffer.assign(gDLL->getText("TXT_KEY_ECON_NET_GOLD"));
}

void CvDLLWidgetData::parseFinanceGoldReserve(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	szBuffer.assign(gDLL->getText("TXT_KEY_ECON_GOLD_RESERVE"));
}
// BULL - Finance Advisor - start
void CvDLLWidgetData::parseFinanceDomesticTrade(CvWidgetDataStruct& widgetDataStruct, CvWStringBuffer& szBuffer) {

	if(widgetDataStruct.m_iData2 > 0) // advc.086: Heading moved into CvGameTextMgr
		GAMETEXT.buildDomesticTradeString(szBuffer, (PlayerTypes)widgetDataStruct.m_iData1);
}

void CvDLLWidgetData::parseFinanceForeignTrade(CvWidgetDataStruct& widgetDataStruct, CvWStringBuffer& szBuffer) {

	if(widgetDataStruct.m_iData2 > 0)  // advc.086: Heading moved into CvGameTextMgr
		GAMETEXT.buildForeignTradeString(szBuffer, (PlayerTypes)widgetDataStruct.m_iData1);
}

void CvDLLWidgetData::parseFinanceSpecialistGold(CvWidgetDataStruct& widgetDataStruct, CvWStringBuffer& szBuffer) {

	// advc.086: No heading
	//szBuffer.assign(gDLL->getText("TXT_KEY_BUG_FINANCIAL_ADVISOR_SPECIALISTS"));
	//szBuffer.append(NEWLINE);
	if(widgetDataStruct.m_iData2 > 0)
		GAMETEXT.buildFinanceSpecialistGoldString(szBuffer, (PlayerTypes)widgetDataStruct.m_iData1);
} // BULL - Finance Advisor - end

void CvDLLWidgetData::parseUnitHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	if (widgetDataStruct.m_iData2 != 0)
	{
		GAMETEXT.setUnitHelp(szBuffer, (UnitTypes)widgetDataStruct.m_iData1, false, false, widgetDataStruct.m_bOption, gDLL->getInterfaceIFace()->getHeadSelectedCity());
	}
}

void CvDLLWidgetData::parsePediaBack(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
//	szBuffer = "Back";
	szBuffer.assign(gDLL->getText("TXT_KEY_MISC_PEDIA_BACK"));
}
// advc.003j (comment): Both unused
void CvDLLWidgetData::parsePediaForward(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
//	szBuffer = "Forward";
	szBuffer.assign(gDLL->getText("TXT_KEY_MISC_PEDIA_FORWARD"));
}

void CvDLLWidgetData::parseBonusHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	if (widgetDataStruct.m_iData2 != 0)
	{
		GAMETEXT.setBonusHelp(szBuffer, (BonusTypes)widgetDataStruct.m_iData1);
	}
}
// BULL - Trade Denial - start
void CvDLLWidgetData::parseBonusTradeHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	if (widgetDataStruct.m_iData2 == -1)
		parseBonusHelp(widgetDataStruct, szBuffer);
	else { /* <advc.073> Hack. Need the bOption field to distinguish between the
			  import (bOption=true) and export columns, but setImageButton
			  (in the EXE) has only the two iData parameters. I'm adding 1000
			  to the bonus id in Python (CvExoticForeignAdvisor: drawResourceDeals)
			  to signal that the widget is in the import column. Proper solution:
			  Two separate widget types - probably wouldn't be that much work to
			  implement either. */
		if(widgetDataStruct.m_iData1 >= 1000) {
			widgetDataStruct.m_iData1 -= 1000;
			widgetDataStruct.m_bOption = true;
		} // </advc.073>
		GAMETEXT.setBonusTradeHelp(szBuffer, (BonusTypes)widgetDataStruct.m_iData1,
			false, (PlayerTypes)widgetDataStruct.m_iData2,
			widgetDataStruct.m_bOption); // advc.073
	}
} // BULL - Trade Denial - end

void CvDLLWidgetData::parseReligionHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	if (widgetDataStruct.m_iData2 != 0)
	{
		GAMETEXT.setReligionHelp(szBuffer, (ReligionTypes)widgetDataStruct.m_iData1);
	}
}

void CvDLLWidgetData::parseReligionHelpCity( CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer )
{
	GAMETEXT.setReligionHelpCity(szBuffer, (ReligionTypes)widgetDataStruct.m_iData1, gDLL->getInterfaceIFace()->getHeadSelectedCity(), true);
}

void CvDLLWidgetData::parseCorporationHelpCity( CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer )
{
	GAMETEXT.setCorporationHelpCity(szBuffer, (CorporationTypes)widgetDataStruct.m_iData1, gDLL->getInterfaceIFace()->getHeadSelectedCity(), true);
}

void CvDLLWidgetData::parseCorporationHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	if (widgetDataStruct.m_iData2 != 0)
	{
		GAMETEXT.setCorporationHelp(szBuffer, (CorporationTypes)widgetDataStruct.m_iData1);
	}
}

void CvDLLWidgetData::parsePromotionHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	if (widgetDataStruct.m_iData2 != 0)
	{
		GAMETEXT.setPromotionHelp(szBuffer, (PromotionTypes)widgetDataStruct.m_iData1);
	}
}

void CvDLLWidgetData::parseEventHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	GAMETEXT.setEventHelp(szBuffer, (EventTypes)widgetDataStruct.m_iData1, widgetDataStruct.m_iData2, GC.getGameINLINE().getActivePlayer());
}

void CvDLLWidgetData::parseUnitCombatHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	if (widgetDataStruct.m_iData2 != 0)
	{
		GAMETEXT.setUnitCombatHelp(szBuffer, (UnitCombatTypes)widgetDataStruct.m_iData1);
	}
}

void CvDLLWidgetData::parseImprovementHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	if (widgetDataStruct.m_iData2 != 0)
	{
		GAMETEXT.setImprovementHelp(szBuffer, (ImprovementTypes)widgetDataStruct.m_iData1);
	}
}

void CvDLLWidgetData::parseCivicHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	if (widgetDataStruct.m_iData2 != 0)
	{
		GAMETEXT.parseCivicInfo(szBuffer, (CivicTypes)widgetDataStruct.m_iData1);
	}
}

void CvDLLWidgetData::parseCivilizationHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	if (widgetDataStruct.m_iData2 != 0)
	{
		GAMETEXT.parseCivInfos(szBuffer, (CivilizationTypes)widgetDataStruct.m_iData1);
	}
}

void CvDLLWidgetData::parseLeaderHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	if (widgetDataStruct.m_iData2 != -1)
	{
		GAMETEXT.parseLeaderTraits(szBuffer, (LeaderHeadTypes)widgetDataStruct.m_iData1, (CivilizationTypes)widgetDataStruct.m_iData2);
	}
}
// BULL - Leaderhead Relations - start
void CvDLLWidgetData::parseLeaderheadRelationsHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer) {

	GAMETEXT.parseLeaderHeadRelationsHelp(szBuffer, (PlayerTypes)widgetDataStruct.m_iData1, (PlayerTypes)widgetDataStruct.m_iData2);
} // BULL - Leaderhead Relations - end
// advc.003j (comment): unused
void CvDLLWidgetData::parseCloseScreenHelp(CvWStringBuffer& szBuffer)
{
	szBuffer.assign(gDLL->getText("TXT_KEY_MISC_CLOSE_SCREEN"));
}

void CvDLLWidgetData::parseDescriptionHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer, bool bMinimal)
{
	CivilopediaPageTypes eType = (CivilopediaPageTypes)widgetDataStruct.m_iData1;
	switch (eType)
	{
	case CIVILOPEDIA_PAGE_TECH:
		{
			TechTypes eTech = (TechTypes)widgetDataStruct.m_iData2;
			if (NO_TECH != eTech)
			{
				szBuffer.assign(bMinimal ? GC.getTechInfo(eTech).getDescription() : gDLL->getText("TXT_KEY_MISC_HISTORICAL_INFO", GC.getTechInfo(eTech).getTextKeyWide()));
			}
		}
		break;
	case CIVILOPEDIA_PAGE_UNIT:
		{
			UnitTypes eUnit = (UnitTypes)widgetDataStruct.m_iData2;
			if (NO_UNIT != eUnit)
			{
				szBuffer.assign(bMinimal ? GC.getUnitInfo(eUnit).getDescription() : gDLL->getText("TXT_KEY_MISC_HISTORICAL_INFO", GC.getUnitInfo(eUnit).getTextKeyWide()));
			}
		}
		break;
	case CIVILOPEDIA_PAGE_BUILDING:
	case CIVILOPEDIA_PAGE_WONDER:
		{
			BuildingTypes eBuilding = (BuildingTypes)widgetDataStruct.m_iData2;
			if (NO_BUILDING != eBuilding)
			{
				szBuffer.assign(bMinimal ? GC.getBuildingInfo(eBuilding).getDescription() : gDLL->getText("TXT_KEY_MISC_HISTORICAL_INFO", GC.getBuildingInfo(eBuilding).getTextKeyWide()));
			}
		}
		break;
	case CIVILOPEDIA_PAGE_BONUS:
		{
			BonusTypes eBonus = (BonusTypes)widgetDataStruct.m_iData2;
			if (NO_BONUS != eBonus)
			{
				szBuffer.assign(bMinimal ? GC.getBonusInfo(eBonus).getDescription() : gDLL->getText("TXT_KEY_MISC_HISTORICAL_INFO", GC.getBonusInfo(eBonus).getTextKeyWide()));
			}
		}
		break;
	case CIVILOPEDIA_PAGE_IMPROVEMENT:
		{
			ImprovementTypes eImprovement = (ImprovementTypes)widgetDataStruct.m_iData2;
			if (NO_IMPROVEMENT != eImprovement)
			{
				szBuffer.assign(bMinimal ? GC.getImprovementInfo(eImprovement).getDescription() : gDLL->getText("TXT_KEY_MISC_HISTORICAL_INFO", GC.getImprovementInfo(eImprovement).getTextKeyWide()));
			}
		}
		break;
	case CIVILOPEDIA_PAGE_UNIT_GROUP:
		{
			UnitCombatTypes eGroup = (UnitCombatTypes)widgetDataStruct.m_iData2;
			if (NO_UNITCOMBAT != eGroup)
			{
				szBuffer.assign(bMinimal ? GC.getUnitCombatInfo(eGroup).getDescription() : gDLL->getText("TXT_KEY_MISC_HISTORICAL_INFO", GC.getUnitCombatInfo(eGroup).getTextKeyWide()));
			}
		}
		break;
	case CIVILOPEDIA_PAGE_PROMOTION:
		{
			PromotionTypes ePromo = (PromotionTypes)widgetDataStruct.m_iData2;
			if (NO_PROMOTION != ePromo)
			{
				szBuffer.assign(bMinimal ? GC.getPromotionInfo(ePromo).getDescription() : gDLL->getText("TXT_KEY_MISC_HISTORICAL_INFO", GC.getPromotionInfo(ePromo).getTextKeyWide()));
			}
		}
		break;
	case CIVILOPEDIA_PAGE_CIV:
		{
			CivilizationTypes eCiv = (CivilizationTypes)widgetDataStruct.m_iData2;
			if (NO_CIVILIZATION != eCiv)
			{
				szBuffer.assign(bMinimal ? GC.getCivilizationInfo(eCiv).getDescription() : gDLL->getText("TXT_KEY_MISC_HISTORICAL_INFO", GC.getCivilizationInfo(eCiv).getTextKeyWide()));
			}
		}
		break;
	case CIVILOPEDIA_PAGE_LEADER:
		{
			LeaderHeadTypes eLeader = (LeaderHeadTypes)widgetDataStruct.m_iData2;
			if (NO_LEADER != eLeader)
			{
				szBuffer.assign(bMinimal ? GC.getLeaderHeadInfo(eLeader).getDescription() : gDLL->getText("TXT_KEY_MISC_HISTORICAL_INFO", GC.getLeaderHeadInfo(eLeader).getTextKeyWide()));
			}
		}
		break;
	case CIVILOPEDIA_PAGE_RELIGION:
		{
			ReligionTypes eReligion = (ReligionTypes)widgetDataStruct.m_iData2;
			if (NO_RELIGION != eReligion)
			{
				szBuffer.assign(bMinimal ? GC.getReligionInfo(eReligion).getDescription() : gDLL->getText("TXT_KEY_MISC_HISTORICAL_INFO", GC.getReligionInfo(eReligion).getTextKeyWide()));
			}
		}
		break;
	case CIVILOPEDIA_PAGE_CORPORATION:
		{
			CorporationTypes eCorporation = (CorporationTypes)widgetDataStruct.m_iData2;
			if (NO_CORPORATION != eCorporation)
			{
				szBuffer.assign(bMinimal ? GC.getCorporationInfo(eCorporation).getDescription() : gDLL->getText("TXT_KEY_MISC_HISTORICAL_INFO", GC.getCorporationInfo(eCorporation).getTextKeyWide()));
			}
		}
		break;
	case CIVILOPEDIA_PAGE_CIVIC:
		{
			CivicTypes eCivic = (CivicTypes)widgetDataStruct.m_iData2;
			if (NO_CIVIC != eCivic)
			{
				szBuffer.assign(bMinimal ? GC.getCivicInfo(eCivic).getDescription() : gDLL->getText("TXT_KEY_MISC_HISTORICAL_INFO", GC.getCivicInfo(eCivic).getTextKeyWide()));
			}
		}
		break;
	case CIVILOPEDIA_PAGE_PROJECT:
		{
			ProjectTypes eProject = (ProjectTypes)widgetDataStruct.m_iData2;
			if (NO_PROJECT != eProject)
			{
				szBuffer.assign(bMinimal ? GC.getProjectInfo(eProject).getDescription() : gDLL->getText("TXT_KEY_MISC_HISTORICAL_INFO", GC.getProjectInfo(eProject).getTextKeyWide()));
			}
		}
		break;
	case CIVILOPEDIA_PAGE_CONCEPT:
		{
			ConceptTypes eConcept = (ConceptTypes)widgetDataStruct.m_iData2;
			if (NO_NEW_CONCEPT != eConcept)
			{
				szBuffer.assign(GC.getConceptInfo(eConcept).getDescription());
			}
		}
		break;
	case CIVILOPEDIA_PAGE_CONCEPT_NEW:
		{
			NewConceptTypes eConcept = (NewConceptTypes)widgetDataStruct.m_iData2;
			if (NO_NEW_CONCEPT != eConcept) // kmodx: was NO_CONCEPT
			{
				szBuffer.assign(GC.getNewConceptInfo(eConcept).getDescription());
			}
		}
		break;
	case CIVILOPEDIA_PAGE_SPECIALIST:
		{
			SpecialistTypes eSpecialist = (SpecialistTypes)widgetDataStruct.m_iData2;
			if (NO_SPECIALIST != eSpecialist)
			{
				szBuffer.assign(bMinimal ? GC.getSpecialistInfo(eSpecialist).getDescription() : gDLL->getText("TXT_KEY_MISC_HISTORICAL_INFO", GC.getSpecialistInfo(eSpecialist).getTextKeyWide()));
			}
		}
		break;
	default:
		break;
	}
}

void CvDLLWidgetData::parseKillDealHelp(CvWidgetDataStruct &widgetDataStruct,
		CvWStringBuffer &szBuffer)
{
	CvWString szTemp = szBuffer.getCString();
	CvGame const& g = GC.getGameINLINE();
	CvDeal* pDeal = g.getDealINLINE(widgetDataStruct.m_iData1);
	if (NULL != pDeal)
	{
		PlayerTypes eActivePlayer = g.getActivePlayer();
		// <advc.073>
		GAMETEXT.getDealString(szBuffer, *pDeal, eActivePlayer, false);
		szBuffer.append(NEWLINE); // </advc.073>
		if (pDeal->isCancelable(eActivePlayer, &szTemp))
		{
			szTemp = gDLL->getText("TXT_KEY_MISC_CLICK_TO_CANCEL");
		}
	}
	szBuffer.append(szTemp); // advc.073: was assign
}


void CvDLLWidgetData::doDealKill(CvWidgetDataStruct &widgetDataStruct)
{
	CvDeal* pDeal = GC.getGameINLINE().getDeal(widgetDataStruct.m_iData1);
	if (pDeal != NULL)
	{
		if (!pDeal->isCancelable(GC.getGameINLINE().getActivePlayer()))
		{
			CvPopupInfo* pInfo = new CvPopupInfo(BUTTONPOPUP_TEXT);
			if (NULL != pInfo)
			{
				pInfo->setText(gDLL->getText("TXT_KEY_POPUP_CANNOT_CANCEL_DEAL"));
				gDLL->getInterfaceIFace()->addPopup(pInfo, GC.getGameINLINE().getActivePlayer(), true);
			}
		}
		else
		{
			CvPopupInfo* pInfo = new CvPopupInfo(BUTTONPOPUP_DEAL_CANCELED);
			if (NULL != pInfo)
			{
				pInfo->setData1(pDeal->getID());
				pInfo->setOption1(false);
				gDLL->getInterfaceIFace()->addPopup(pInfo, GC.getGameINLINE().getActivePlayer(), true);
			}
		}
	}
}


void CvDLLWidgetData::doRefreshMilitaryAdvisor(CvWidgetDataStruct &widgetDataStruct)
{
	CyArgsList argsList;
	argsList.add(widgetDataStruct.m_iData1);
	argsList.add(widgetDataStruct.m_iData2);
	gDLL->getPythonIFace()->callFunction(PYScreensModule, "refreshMilitaryAdvisor", argsList.makeFunctionArgs());
}

void CvDLLWidgetData::parseProductionModHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	CvCity* pCity = gDLL->getInterfaceIFace()->getHeadSelectedCity();
	if (NULL != pCity)
	{
		GAMETEXT.setProductionHelp(szBuffer, *pCity);
	}
}

void CvDLLWidgetData::parseLeaderheadHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	GAMETEXT.parseLeaderHeadHelp(szBuffer, (PlayerTypes)widgetDataStruct.m_iData1, (PlayerTypes)widgetDataStruct.m_iData2);
}

void CvDLLWidgetData::parseLeaderLineHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	GAMETEXT.parseLeaderLineHelp(szBuffer, (PlayerTypes)widgetDataStruct.m_iData1, (PlayerTypes)widgetDataStruct.m_iData2);
}

void CvDLLWidgetData::parseCommerceModHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	CvCity* pCity = gDLL->getInterfaceIFace()->getHeadSelectedCity();
	if (NULL != pCity)
	{
		GAMETEXT.setCommerceHelp(szBuffer, *pCity, (CommerceTypes)widgetDataStruct.m_iData1);
	}
}

void CvDLLWidgetData::parseScoreHelp(CvWidgetDataStruct& widgetDataStruct, CvWStringBuffer& szBuffer)
{
	GAMETEXT.setScoreHelp(szBuffer, (PlayerTypes)widgetDataStruct.m_iData1);
}

// BULL - Trade Hover - start
void CvDLLWidgetData::parseTradeRoutes(CvWidgetDataStruct& widgetDataStruct, CvWStringBuffer& szBuffer) {

	GAMETEXT.buildTradeString(szBuffer, (PlayerTypes)widgetDataStruct.m_iData1, (PlayerTypes)widgetDataStruct.m_iData2);
	GAMETEXT.getActiveDealsString(szBuffer, (PlayerTypes)widgetDataStruct.m_iData1, (PlayerTypes)widgetDataStruct.m_iData2,
			true); // advc.087
} // BULL - Trade Hover - end
// BULL - Food Rate Hover - start
void CvDLLWidgetData::parseFoodModHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer) {

	CvCity const* pCity = gDLL->getInterfaceIFace()->getHeadSelectedCity();
	if(pCity == NULL)
		return;
	GAMETEXT.setFoodHelp(szBuffer, *pCity);
}
// BUG - Food Rate Hover - end
// <advc.085>
void CvDLLWidgetData::parsePowerRatioHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer) {

	CvPlayer const& kPlayer = GET_PLAYER((PlayerTypes)widgetDataStruct.m_iData1);
	CvPlayer const& kActivePlayer = GET_PLAYER(GC.getGameINLINE().getActivePlayer());
	bool bThemVsYou = (getBugOptionINT("Scores__PowerFormula", 0, false) == 0);
	int iPow = std::max(1, kPlayer.getPower());
	int iActivePow = std::max(1, kActivePlayer.getPower());
	int iPowerRatioPercent = ::round(bThemVsYou ?
			(100.0 * iPow) / iActivePow : (100.0 * iActivePow) / iPow);
	CvWString szCompareTag("TXT_KEY_POWER_RATIO_");
	if(iPowerRatioPercent < 100)
		szCompareTag += L"SMALLER";
	else if(iPowerRatioPercent > 100)
		szCompareTag += L"GREATER";
	else szCompareTag += L"EQUAL";
	CvWString szPowerRatioHelpTag("TXT_KEY_POWER_RATIO_HELP_");
	if(bThemVsYou)
		szPowerRatioHelpTag += L"THEM_VS_YOU";
	else szPowerRatioHelpTag += L"YOU_VS_THEM";
	szBuffer.append(gDLL->getText(szPowerRatioHelpTag, kPlayer.getName(),
			gDLL->getText(szCompareTag).GetCString()));
	ColorTypes eRatioColor = widgetDataStruct.m_iData2 <= 0 ? NO_COLOR :
			(ColorTypes)widgetDataStruct.m_iData2;
	if(eRatioColor != NO_COLOR) {
		NiColorA const& kRatioColor = GC.getColorInfo(eRatioColor).getColor();
		szBuffer.append(CvWString::format(L" " SETCOLR L"%d%%" ENDCOLR,
				// Based on the TEXT_COLOR macro
				(int)(kRatioColor.r * 255), (int)(kRatioColor.g * 255),
				(int)(kRatioColor.b * 255), (int)(kRatioColor.a * 255),
				iPowerRatioPercent));
	}
	else szBuffer.append(CvWString::format(L" %d%%", iPowerRatioPercent));
	if(kActivePlayer.getMasterTeam() == kPlayer.getMasterTeam())
		return; // Espionage vs. allies isn't so interesting
	int iNeededDemographics = kActivePlayer.espionageNeededToSee(kPlayer.getID(), true);
	// (Might make more sense to show this in the tech hover)
	int iNeededResearch = kActivePlayer.espionageNeededToSee(kPlayer.getID(), false);
	FAssert((iNeededDemographics <= 0 && iNeededDemographics != MAX_INT) ||
			GC.getGameINLINE().isDebugMode());
	if(iNeededDemographics == MAX_INT)
		return;
	szBuffer.append(NEWLINE);
	CvWString szSeeWhat;
	int iNeeded = iNeededDemographics;
	if(iNeededResearch <= 0) {
		iNeeded = iNeededResearch;
		szSeeWhat = gDLL->getText("TXT_KEY_COMMERCE_RESEARCH");
	}
	else szSeeWhat = gDLL->getText("TXT_KEY_DEMO_SCREEN_TITLE");
	szBuffer.append(gDLL->getText("TXT_KEY_SEE_INTEL_THRESHOLD",
			szSeeWhat.GetCString(), -iNeeded));
}


void CvDLLWidgetData::parseGoldenAgeAnarchyHelp(PlayerTypes ePlayer, int iData2,
		bool bAnarchy, CvWStringBuffer &szBuffer) {

	CvPlayer const& kPlayer = GET_PLAYER(ePlayer);
	if(bAnarchy) {
		int iTurns = kPlayer.getAnarchyTurns();
		FAssert(iTurns > 0);
		szBuffer.append(gDLL->getText("INTERFACE_ANARCHY", iTurns));
	}
	else {
		szBuffer.append(gDLL->getText("TXT_KEY_CONCEPT_GOLDEN_AGE"));
		int iTurns = kPlayer.getGoldenAgeTurns();
		FAssert(iTurns > 0);
		szBuffer.append(CvWString::format(L" (%s)",
				gDLL->getText("TXT_KEY_MISC_TURNS_LEFT2", iTurns).GetCString()));
	}
} // </advc.085>

/*  K-Mod, 5/jan/11, karadoc
	Environmental advisor mouse-over text */
void CvDLLWidgetData::parsePollutionOffsetsHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	szBuffer.append(gDLL->getText("TXT_KEY_POLLUTION_OFFSETS_HELP"));

	for (int iI = 0; iI < GC.getNumFeatureInfos(); ++iI)
	{
		int iWarmingDefence = GC.getFeatureInfo((FeatureTypes)iI).getWarmingDefense();

		if (iWarmingDefence != 0)
		{
			szBuffer.append(NEWLINE);
			szBuffer.append(gDLL->getText("TXT_KEY_OFFSET_PER_FEATURE", -iWarmingDefence, GC.getFeatureInfo((FeatureTypes)iI).getTextKeyWide()));
		}
	}
}

void CvDLLWidgetData::parsePollutionHelp(CvWidgetDataStruct &widgetDataStruct, CvWStringBuffer &szBuffer)
{
	int iFlags = (int)widgetDataStruct.m_iData1;

	szBuffer.append(gDLL->getText("TXT_KEY_POLLUTION")+":");

	if (iFlags & POLLUTION_POPULATION)
	{
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_POLLUTION_FROM_POPULATION", GC.getDefineINT("GLOBAL_WARMING_POPULATION_WEIGHT")));
	}
	if (iFlags & POLLUTION_BUILDINGS)
	{
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_POLLUTION_FROM_BUILDINGS", GC.getDefineINT("GLOBAL_WARMING_BUILDING_WEIGHT")));
	}
	if (iFlags & POLLUTION_BONUSES)
	{
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_POLLUTION_FROM_BONUSES", GC.getDefineINT("GLOBAL_WARMING_BONUS_WEIGHT")));
	}
	if (iFlags & POLLUTION_POWER)
	{
		szBuffer.append(NEWLINE);
		szBuffer.append(gDLL->getText("TXT_KEY_POLLUTION_FROM_POWER", GC.getDefineINT("GLOBAL_WARMING_POWER_WEIGHT")));
	}
} // K-Mod end

// <advc.004a>
CvWString CvDLLWidgetData::getDiscoverPathText(UnitTypes eUnit, PlayerTypes ePlayer) const {

	CvWString r(L"\n");
	/*  Could have ported the code in BUG TechPrefs.py, but it's unnecessarily
		complicated for what I'm trying to do. Use ::getDiscoveryTech and,
		in between calls, pretend that the previous tech has already been discovered. */
	TechTypes eCurrentDiscover = ::getDiscoveryTech(eUnit, ePlayer);
	if(eCurrentDiscover == NO_TECH || eUnit == NO_UNIT)
		return r;
	FlavorTypes eGPFlavor = NO_FLAVOR;
	int iMaxFlavor = -1;
	CvUnitInfo& u = GC.getUnitInfo(eUnit);
	for(int i = 0; i < GC.getNumFlavorTypes(); i++) {
		int iFlavor = u.getFlavorValue(i);
		if(iFlavor > iMaxFlavor) {
			eGPFlavor = (FlavorTypes)i;
			iMaxFlavor = iFlavor;
		}
	}
	CvString szFlavor;
	switch(eGPFlavor) {
		case FLAVOR_SCIENCE: szFlavor = "SCIENCE"; break;
		case FLAVOR_MILITARY: szFlavor = "MILITARY"; break;
		case FLAVOR_RELIGION: szFlavor = "RELIGION"; break;
		case FLAVOR_PRODUCTION: szFlavor = "PRODUCTION"; break;
		case FLAVOR_GOLD: szFlavor = "GOLD"; break;
		case FLAVOR_CULTURE: szFlavor = "CULTURE"; break;
		case FLAVOR_GROWTH: szFlavor = "GROWTH"; break;
		default: FAssert(false); return r;
	}
	r.append(gDLL->getText("TXT_KEY_MISSION_DISCOVER_HELP1"));
	r.append(L" ");
	CvString szFlavorKey("TXT_KEY_FLAVOR_" + szFlavor + "_TECH");
	r.append(gDLL->getText(szFlavorKey));
	r.append(L". ");
	CvTeam& kTeam = TEAMREF(ePlayer);
	/*  The same discovery could be enabled by multiple currently researchable techs.
		The map lists the alt. reqs for each target tech. */
	std::map<TechTypes,std::set<TechTypes>*> discoverMap;
	for(int i = 0; i < GC.getNumTechInfos(); i++) {
		TechTypes eResearchOption = (TechTypes)i;
		if(!GET_PLAYER(ePlayer).canResearch(eResearchOption))
			continue;
		kTeam.setHasTechTemporarily(eResearchOption, true);
		TechTypes eNextDiscover = ::getDiscoveryTech(eUnit, ePlayer);
		kTeam.setHasTechTemporarily(eResearchOption, false);
		if(eNextDiscover == eCurrentDiscover || eNextDiscover == NO_TECH)
			continue;
		std::set<TechTypes>* discoverSet = NULL;
		if(discoverMap.find(eNextDiscover) == discoverMap.end()) {
			discoverSet = new std::set<TechTypes>();
			discoverMap.insert(std::make_pair<TechTypes,std::set<TechTypes>*>(
					eNextDiscover, discoverSet));
		}
		else discoverSet = discoverMap.find(eNextDiscover)->second;
		discoverSet->insert(eResearchOption);
	}
	if(discoverMap.empty())
		return r;
	r.append(gDLL->getText("TXT_KEY_MISSION_DISCOVER_HELP2"));
	r.append(L" ");
	if(discoverMap.size() == 1)
		r.append(gDLL->getText(szFlavorKey));
	else {
		CvString szPluralKey = "TXT_KEY_FLAVOR_" + szFlavor + "_PLURAL";
		r.append(gDLL->getText(szPluralKey));
	}
	r.append(L": ");
	for(std::map<TechTypes,std::set<TechTypes>*>::iterator it = discoverMap.begin();
			it != discoverMap.end(); it++) {
		if(it != discoverMap.begin())
			r.append(L", ");
		CvTechInfo const& kNextDiscover = GC.getTechInfo(it->first);
		CvWString szTemp;
		szTemp.Format(SETCOLR L"%s" ENDCOLR, TEXT_COLOR("COLOR_TECH_TEXT"),
				kNextDiscover.getDescription());
		r.append(szTemp);
		r.append(L" (");
		r.append(gDLL->getText("TXT_KEY_MISSION_DISCOVER_REQ"));
		r.append(L" ");
		std::set<TechTypes> discoverSet = *it->second;
		for(std::set<TechTypes>::iterator sit = discoverSet.begin();
				sit != discoverSet.end(); sit++) {
			if(sit != discoverSet.begin()) {
				r.append(L" ");
				r.append(gDLL->getText("TXT_KEY_MISSION_DISCOVER_OR"));
				r.append(L" ");
			}
			CvTechInfo const& kReqTech = GC.getTechInfo(*sit);
			szTemp.Format(SETCOLR L"%s" ENDCOLR, TEXT_COLOR("COLOR_TECH_TEXT"),
					kReqTech.getDescription());
			r.append(szTemp);
		}
		r.append(L")");
		delete it->second;
	}
	r.append(L".");
	return r;
} // </advc.004a>

// <advc.004b>
CvWString CvDLLWidgetData::getFoundCostText(CvPlot const& p, PlayerTypes eOwner) const {

	CvPlayer& kOwner = GET_PLAYER(eOwner);
	if(kOwner.isAnarchy())
		return "";
	int iProjPreInfl = 0;
	// New city increases other cities' maintenance
	int foo=-1;
	for(CvCity* c = kOwner.firstCity(&foo); c != NULL; c = kOwner.nextCity(&foo)) {
		if(c->isDisorder()) // Can't account for these
			continue;
		int iProjected = // Distance and corp. maintenance stay the same
				c->calculateDistanceMaintenanceTimes100() +
				c->calculateCorporationMaintenanceTimes100() +
				CvCity::calculateNumCitiesMaintenanceTimes100(*c->plot(),
				eOwner, c->getPopulation(), 1) +
				CvCity::calculateColonyMaintenanceTimes100(*c->plot(),
				eOwner, c->getPopulation(), 1);
		// Snippet from CvCity::updateMaintenance
		iProjPreInfl += (iProjected *
				std::max(0, c->getMaintenanceModifier() + 100)) / 100;
	}
	int iNewCityMaint = 
			CvCity::calculateDistanceMaintenanceTimes100(p, eOwner) +
			// Last param: +1 for the newly founded city
			CvCity::calculateNumCitiesMaintenanceTimes100(p, eOwner, -1, 1) +
			CvCity::calculateColonyMaintenanceTimes100(p, eOwner);
	iProjPreInfl += iNewCityMaint;
	iProjPreInfl /= 100;
	// Civic upkeep
	iProjPreInfl += kOwner.getCivicUpkeep(NULL, true, 1);
	// Unit cost (new city increases free units, Settler unit goes away)
	iProjPreInfl += kOwner.calculateUnitCost(CvCity::initialPopulation(), -1);
	// Unit supply (Settler unit goes away)
	iProjPreInfl += kOwner.calculateUnitSupply(p.getOwner() == eOwner ? 0 : -1);
	// Inflation
	int iCost = (iProjPreInfl * (kOwner.calculateInflationRate() + 100)) / 100;
	// Difference from current expenses
	iCost -= kOwner.calculateInflatedCosts();
	/* Could, in theory, be negative due to unit cost. Don't output
	   a negative cost (too confusing). */
	iCost = std::max(0, iCost);
	CvWString r = L"\n";
	CvWString costStr = CvWString::format(L"%d", iCost);
	r.append(gDLL->getText("TXT_KEY_PROJECTED_COST", costStr.GetCString()));
	return r;
}

CvWString CvDLLWidgetData::getNetFeatureHealthText(CvPlot const& kCityPlot,
		PlayerTypes eOwner) const {

	int iGoodHealthPercent = 0;
	int iBadHealthPercent = 0;
	for(int i = 0; i < NUM_CITY_PLOTS; i++) {
		if(i == CITY_HOME_PLOT) // Feature gets removed upon founding
			continue;
		CvPlot* pPlot = plotCity(kCityPlot.getX_INLINE(), kCityPlot.getY_INLINE(), i);
		if(pPlot == NULL) continue; CvPlot const& p = *pPlot;
		if(p.getFeatureType() == NO_FEATURE || !p.isRevealed(TEAMID(eOwner), false))
			continue;
		int iHealthPercent = GC.getFeatureInfo(p.getFeatureType()).getHealthPercent();
		if(iHealthPercent > 0)
			iGoodHealthPercent += iHealthPercent;
		else iBadHealthPercent -= iHealthPercent;
	}
	CvWString r;
	if(kCityPlot.isFreshWater()) {
		r.append(NEWLINE);
		r.append(gDLL->getText("TXT_KEY_MISC_HEALTH_FROM_FRESH_WATER",
				GC.getDefineINT("FRESH_WATER_HEALTH_CHANGE")));
	}
	int iGoodHealth = iGoodHealthPercent / 100;
	int iBadHealth = iBadHealthPercent / 100;
	if(iGoodHealth > 0 || iBadHealth > 0) {
		r.append(NEWLINE);
		r.append(L"+");
		int icon = 0;
		if(iGoodHealth > 0) {
			icon = gDLL->getSymbolID(HEALTHY_CHAR);
			/*  Turns out good and bad health are rounded individually;
				no need, then, to show fractions. */
			// float goodHealth = goodHealthPercent / 100.0f;
			//r.append(CvWString::format((goodHealthPercent % 10 == 0 ?
			//		L"%.1f%c" : L"%.2f%c"), goodHealth, icon));
			r.append(CvWString::format(L"%d%c", iGoodHealth, icon));
		}
		if(iBadHealth > 0) {
			if(iGoodHealth > 0)
				r.append(CvWString::format(L", "));
			icon = gDLL->getSymbolID(UNHEALTHY_CHAR);
			r.append(CvWString::format(L"%d%c", iBadHealth, icon));
		}
		r.append(gDLL->getText("TXT_KEY_FROM_FEATURES"));
	}
	int iExtraHealth = GET_PLAYER(eOwner).getExtraHealth();
	if(iExtraHealth != 0) {
		r.append(NEWLINE);
		int iIcon = 0;
		r.append(L"+");
		if(iExtraHealth > 0) {
			iIcon = gDLL->getSymbolID(HEALTHY_CHAR);
			r.append(CvWString::format(L"%d%c", iExtraHealth, iIcon));
		}
		else {
			iIcon = gDLL->getSymbolID(UNHEALTHY_CHAR);
			r.append(CvWString::format(L"%d%c", iBadHealth, iIcon));
		}
		r.append(gDLL->getText("TXT_KEY_FROM_TRAIT"));
	}
	return r;
}

CvWString CvDLLWidgetData::getHomePlotYieldText(CvPlot const& p, PlayerTypes eOwner) const {

	CvWString r = NEWLINE;
	r.append(gDLL->getText("TXT_KEY_HOME_TILE_YIELD"));
	for(int i = 0; i < NUM_YIELD_TYPES; i++) {
		YieldTypes eYield = (YieldTypes)i;
		int y = p.calculateNatureYield(eYield, TEAMID(eOwner), true);
		CvYieldInfo& kYield = GC.getYieldInfo(eYield);
		y = std::max(y, kYield.getMinCity());
		if(y == 0)
			continue;
		CvWString szYield = CvWString::format(L", %d%c", y, kYield.getChar());
		r.append(szYield);
	}
	return r;
} // </advc.004b>
