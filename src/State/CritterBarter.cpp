/*
 * Copyright 2012-2016 Falltergeist Developers.
 *
 * This file is part of Falltergeist.
 *
 * Falltergeist is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Falltergeist is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Falltergeist.  If not, see <http://www.gnu.org/licenses/>.
 */

// Related headers
#include "../State/CritterBarter.h"

// C++ standard includes

// Falltergeist includes
#include "../Game/Game.h"
#include "../Graphics/Renderer.h"
//#include "../State/CritterDialog.h"
#include "../State/CritterInteract.h"
#include "../UI/Image.h"
#include "../UI/ImageButton.h"
#include "../Game/ContainerItemObject.h"
#include "../Game/DudeObject.h"
#include "../Input/Mouse.h"
#include "../Logger.h"
#include "../UI/Image.h"
#include "../UI/ImageButton.h"
#include "../UI/ItemsList.h"
#include "../UI/TextArea.h"

// Third party includes

namespace Falltergeist
{
    namespace State
    {
        CritterBarter::CritterBarter() : State()
        {
			_playerContainer = new Game::ContainerItemObject();
			_traderContainer = new Game::ContainerItemObject();
        }

        CritterBarter::~CritterBarter()
        {
			if (_playerContainer) {
				delete _playerContainer;
				_playerContainer = nullptr;
			}
			if (_traderContainer) {
				delete _traderContainer;
				_traderContainer = nullptr;
			}
        }

        void CritterBarter::init()
        {
            if (_initialized) return;
            State::init();

            setFullscreen(false);
            setModal(false);

            setPosition((Game::getInstance()->renderer()->size() - Point(640, 480)) / 2 + Point(0, 291));

            addUI("background",new UI::Image("art/intrface/barter.frm"));

            addUI("offerButton", new UI::ImageButton(UI::ImageButton::Type::DIALOG_RED_BUTTON, 40, 162));
			getUI("offerButton")->mouseClickHandler().add(std::bind(&CritterBarter::onOfferButtonClick, this, std::placeholders::_1));

            addUI("talkButton", new UI::ImageButton(UI::ImageButton::Type::DIALOG_RED_BUTTON, 583, 162));
            getUI("talkButton")->mouseClickHandler().add(std::bind(&CritterBarter::onTalkButtonClick, this, std::placeholders::_1));

            addUI("mineInventoryScrollUpButton",   new UI::ImageButton(UI::ImageButton::Type::DIALOG_UP_ARROW,   190, 56));
            addUI("mineInventoryScrollDownButton", new UI::ImageButton(UI::ImageButton::Type::DIALOG_DOWN_ARROW, 190, 82));
			getUI("mineInventoryScrollUpButton")->mouseClickHandler().add(std::bind(&CritterBarter::onPlayerScrollUpButtonClick, this, std::placeholders::_1));
			getUI("mineInventoryScrollDownButton")->mouseClickHandler().add(std::bind(&CritterBarter::onPlayerScrollDownButtonClick, this, std::placeholders::_1));

            addUI("theirsInventoryScrollUpButton",   new UI::ImageButton(UI::ImageButton::Type::DIALOG_UP_ARROW,   421, 56));
            addUI("theirsInventoryScrollDownButton", new UI::ImageButton(UI::ImageButton::Type::DIALOG_DOWN_ARROW, 421, 82));
			getUI("theirsInventoryScrollUpButton")->mouseClickHandler().add(std::bind(&CritterBarter::onCritterScrollUpButtonClick, this, std::placeholders::_1));
			getUI("theirsInventoryScrollDownButton")->mouseClickHandler().add(std::bind(&CritterBarter::onCritterScrollDownButtonClick, this, std::placeholders::_1));

			//Player
			auto playerInvList = new UI::ItemsList({ 110, 30 });
			playerInvList->setItems(Game::getInstance()->player()->inventory());
			playerInvList->setSlotsNumber(4);
			addUI("player_inv_list", playerInvList);

			auto playerContainerList = new UI::ItemsList({ 240, 20 });
			playerContainerList->setItems(_playerContainer->inventory());
			playerContainerList->setSlotsNumber(4);
			addUI("player_container_list", playerContainerList);

			auto playercontainerWorth = new UI::TextArea("0",240, 10);
			addUI("PlayerWorth",playercontainerWorth);

			//Trader
			auto traderInvList = new UI::ItemsList({ 470, 30 });
			traderInvList->setItems(trader()->inventory());
			traderInvList->setSlotsNumber(4);
			addUI("trader_inv_list", traderInvList);

			auto traderContainerList = new UI::ItemsList({ 330, 20 });
			traderContainerList->setItems(_traderContainer->inventory());
			traderContainerList->setSlotsNumber(4);
			addUI("trader_container_list", traderContainerList);
			
			auto tradercontainerWorth = new UI::TextArea("0", 330, 10);
			addUI("TraderWorth", tradercontainerWorth);

			//Player
			playerInvList->itemDragStopHandler().add([playerContainerList](Event::Mouse* event) { playerContainerList->onItemDragStop(event); });
			playerContainerList->itemDragStopHandler().add([playerInvList](Event::Mouse* event) { playerInvList->onItemDragStop(event); });
			playerContainerList->itemsListModifiedHandler().add([this](Event::Event* event) {this->onPlayerContainerDrop(event); });
			//Trader
			traderInvList->itemDragStopHandler().add([traderContainerList](Event::Mouse* event) { traderContainerList->onItemDragStop(event); });
			traderContainerList->itemDragStopHandler().add([traderInvList](Event::Mouse* event) { traderInvList->onItemDragStop(event); });
			traderContainerList->itemsListModifiedHandler().add([this](Event::Event* event) {this->onTraderContainerDrop(event); });
        }

		void CritterBarter::onPlayerContainerDrop(Event::Event* event) {
			auto playerworth= dynamic_cast<UI::TextArea*>(getUI("PlayerWorth"));
			int value = 0;
			auto inv = _playerContainer->inventory();
			for (auto v : *inv) {
				value += v->price()*v->amount();
			}
			if (playerworth) {
				playerworth->setText(std::to_string(value));
			}
		}

		void CritterBarter::onTraderContainerDrop(Event::Event* event) {
			auto traderworth = dynamic_cast<UI::TextArea*>(getUI("TraderWorth"));
			int value = 0;
			auto inv = _traderContainer->inventory();
			for (auto v : *inv) {
				value += v->price()*v->amount();
			}
			if (traderworth) {
				traderworth->setText(std::to_string(value));
			}
		}

		void CritterBarter::ResetBarter() {
			{
				auto container = _traderContainer->inventory();
				auto inventory = trader()->inventory();
				for (auto v : *container)inventory->push_back(v);
				container->clear();
				dynamic_cast<UI::ItemsList*>(getUI("trader_inv_list"))->setItems(inventory);
				dynamic_cast<UI::ItemsList*>(getUI("trader_container_list"))->setItems(container);
				dynamic_cast<UI::TextArea*>(getUI("TraderWorth"))->setText("");
			}
			{
				auto container = _playerContainer->inventory();
				auto inventory = Game::getInstance()->player()->inventory();
				for (auto v : *container)inventory->push_back(v);
				container->clear();
				dynamic_cast<UI::ItemsList*>(getUI("player_inv_list"))->setItems(inventory);
				dynamic_cast<UI::ItemsList*>(getUI("player_container_list"))->setItems(container);
				dynamic_cast<UI::TextArea*>(getUI("PlayerWorth"))->setText("");
			}
		}

		void CritterBarter::onOfferButtonClick(Event::Mouse* event)
		{
			int  tradeMoney = std::stoi(dynamic_cast<UI::TextArea*>(getUI("TraderWorth"))->text());
			int playerMoney = std::stoi(dynamic_cast<UI::TextArea*>(getUI("PlayerWorth"))->text());
			if (tradeMoney < playerMoney) {
				auto traderContainer = _traderContainer->inventory();
				auto playerContainer = _playerContainer->inventory();
				auto traderInventory = trader()->inventory();
				auto playerInventory = Game::getInstance()->player()->inventory();
				for (auto v : *traderContainer)playerInventory->push_back(v);
				for (auto v : *playerContainer)traderInventory->push_back(v);
				traderContainer->clear();
				playerContainer->clear();

				dynamic_cast<UI::ItemsList*>(getUI("trader_inv_list"))->setItems(traderInventory);
				dynamic_cast<UI::ItemsList*>(getUI("trader_container_list"))->setItems(traderContainer);
				dynamic_cast<UI::TextArea*>(getUI("TraderWorth"))->setText("0");
				dynamic_cast<UI::ItemsList*>(getUI("player_inv_list"))->setItems(playerInventory);
				dynamic_cast<UI::ItemsList*>(getUI("player_container_list"))->setItems(playerContainer);
				dynamic_cast<UI::TextArea*>(getUI("TraderWorth"))->setText("0");
				dynamic_cast<UI::TextArea*>(getUI("PlayerWorth"))->setText("0");
			}
			else if (tradeMoney == playerMoney) {
				auto traderContainer = _traderContainer->inventory();
				auto playerContainer = _playerContainer->inventory();
				auto traderInventory = trader()->inventory();
				auto playerInventory = Game::getInstance()->player()->inventory();
				for (auto v : *traderContainer)playerInventory->push_back(v);
				for (auto v : *playerContainer)traderInventory->push_back(v);
				traderContainer->clear();
				playerContainer->clear();

				dynamic_cast<UI::ItemsList*>(getUI("trader_inv_list"))->setItems(traderInventory);
				dynamic_cast<UI::ItemsList*>(getUI("trader_container_list"))->setItems(traderContainer);
				dynamic_cast<UI::TextArea*>(getUI("TraderWorth"))->setText("0");
				dynamic_cast<UI::ItemsList*>(getUI("player_inv_list"))->setItems(playerInventory);
				dynamic_cast<UI::ItemsList*>(getUI("player_container_list"))->setItems(playerContainer);
				dynamic_cast<UI::TextArea*>(getUI("TraderWorth"))->setText("0");
				dynamic_cast<UI::TextArea*>(getUI("PlayerWorth"))->setText("0");
			}else {

			}
		}

        void CritterBarter::onTalkButtonClick(Event::Mouse* event)
        {
            if (auto interact = dynamic_cast<CritterInteract*>(Game::getInstance()->topState(1)))
            {
				ResetBarter();
                interact->switchSubState(CritterInteract::SubState::DIALOG);
            }
        }

        void CritterBarter::onBackgroundClick(Event::Mouse* event)
        {
        }

		void CritterBarter::onPlayerScrollUpButtonClick(Event::Mouse* event)
		{
			auto inventory = dynamic_cast<UI::ItemsList*>(getUI("player_inv_list"));
			if (inventory->canScrollUp())
			{
				inventory->scrollUp();
			}
		}
	
		void CritterBarter::onPlayerScrollDownButtonClick(Event::Mouse* event)
		{
			auto inventory = dynamic_cast<UI::ItemsList*>(getUI("player_inv_list"));
			if (inventory->canScrollDown())
			{
				inventory->scrollDown();
			}
		}

		void CritterBarter::onCritterScrollUpButtonClick(Event::Mouse* event)
		{
			auto inventory = dynamic_cast<UI::ItemsList*>(getUI("trader_inv_list"));
			if (inventory->canScrollUp())
			{
				inventory->scrollUp();
			}
		}

		void CritterBarter::onCritterScrollDownButtonClick(Event::Mouse* event)
		{
			auto inventory = dynamic_cast<UI::ItemsList*>(getUI("trader_inv_list"));
			if (inventory->canScrollDown())
			{
				inventory->scrollDown();
			}
		}

		Game::CritterObject* CritterBarter::trader()
		{
			return _trader;
		}

		void CritterBarter::setTrader(Game::CritterObject* trader)
		{
			_trader = trader;
		}
    }
}
