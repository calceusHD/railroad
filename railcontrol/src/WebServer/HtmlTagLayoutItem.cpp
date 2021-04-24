/*
RailControl - Model Railway Control Software

Copyright (c) 2017-2021 Dominik (Teddy) Mahrer - www.railcontrol.org

RailControl is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the
Free Software Foundation; either version 3, or (at your option) any
later version.

RailControl is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with RailControl; see the file LICENCE. If not see
<http://www.gnu.org/licenses/>.
*/

#include <string>

#include "WebServer/HtmlTagLayoutItem.h"

using std::string;
using std::to_string;

namespace WebServer
{
	const string HtmlTagLayoutItem::EdgeLengthString = std::to_string(HtmlTagLayoutItem::EdgeLength);

	HtmlTagLayoutItem::HtmlTagLayoutItem(const DataModel::LayoutItem* layout)
	:	layout(layout),
		imageDiv("div"),
		onClickMenuDiv("div"),
		onClickMenuContentDiv("ul"),
		contextMenuDiv("div"),
		contextMenuContentDiv("ul"),
		layoutPosX(layout->GetPosX() * EdgeLength),
		layoutPosY(layout->GetPosY() * EdgeLength)
	{
		switch (layout->GetObjectType())
		{
			case ObjectTypeTrack:
				identifier = "t_";
				break;

			case ObjectTypeSignal:
				identifier = "si_";
				break;

			case ObjectTypeSwitch:
				identifier = "sw_";
				break;

			default:
				identifier = "unknown_";
				return;
		}
		identifier += to_string(layout->GetID());
		imageDiv.AddId(identifier);
		imageDiv.AddClass("layout_item");
		imageDiv.AddAttribute("style", "left:" + to_string(layoutPosX) + "px;top:" + to_string(layoutPosY) + "px;");

		string menuPosition = "left:" + to_string(layoutPosX + 5) + "px;top:" + to_string(layoutPosY + 30) + "px;";
		onClickMenuDiv.AddClass("contextmenu");
		onClickMenuDiv.AddId(identifier + "_onclick");
		onClickMenuDiv.AddAttribute("style", menuPosition);
		onClickMenuContentDiv.AddClass("contextentries");

		contextMenuDiv.AddClass("contextmenu");
		contextMenuDiv.AddId(identifier + "_context");
		contextMenuDiv.AddAttribute("style", menuPosition);
		contextMenuContentDiv.AddClass("contextentries");
	}

	void HtmlTagLayoutItem::FinishInit()
	{
		const DataModel::LayoutItem::LayoutItemSize trackHeight = layout->GetHeight();
		const string layoutHeight = to_string(EdgeLength * trackHeight);

		int translate = 0;
		if (trackHeight > DataModel::LayoutItem::Height1)
		{
			DataModel::LayoutItem::LayoutRotation trackRotation = layout->GetRotation();
			if (trackRotation == DataModel::LayoutItem::Rotation90 || trackRotation == DataModel::LayoutItem::Rotation270)
			{
				translate = (((trackHeight - 1) * EdgeLength) / 2);
			}
			if (trackRotation == DataModel::LayoutItem::Rotation90)
			{
				translate = -translate;
			}
		}

		imageDiv.AddChildTag(HtmlTag().AddContent("<svg width=\"" + EdgeLengthString + "\" height=\"" + layoutHeight + "\" id=\"" + identifier + "_img\" style=\"transform:rotate(" + DataModel::LayoutItem::Rotation(layout->GetRotation()) + "deg) translate(" + to_string(translate) + "px," + to_string(translate) + "px);\">" + image + "</svg>"));
		imageDiv.AddAttribute("oncontextmenu", "return showContextMenu(event, '" + identifier + "');");

		if (onClickMenuContentDiv.ChildCount())
		{
			imageDiv.AddAttribute("onclick", "return showOnClickMenu(event, '" + identifier + "');");
			onClickMenuDiv.AddChildTag(onClickMenuContentDiv);
			AddChildTag(onClickMenuDiv);
		}
		contextMenuDiv.AddChildTag(contextMenuContentDiv);
		AddChildTag(contextMenuDiv);
		AddChildTag(imageDiv);
	}

	void HtmlTagLayoutItem::AddMenuEntry(HtmlTag& menu,
		const string& text)
	{
		HtmlTag li("li");
		li.AddClass("contextentry");
		li.AddContent(text);
		menu.AddChildTag(li);
	}

	void HtmlTagLayoutItem::AddMenuEntry(HtmlTag& menu,
		const Languages::TextSelector text,
		const string& onClick,
		const string& className)
	{
		HtmlTag li("li");
		li.AddClass("contextentry");
		li.AddContent(text);
		if (onClick.length() > 0)
		{
			li.AddAttribute("onClick", onClick);
		}
		if (className.length() > 0)
		{
			li.AddClass(className);
		}
		menu.AddChildTag(li);
	}
} // namespace WebServer