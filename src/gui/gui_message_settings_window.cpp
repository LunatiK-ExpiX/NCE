#include "gui_message_settings_window.hpp"
#include "gui_element_types.hpp"
#include "gui_element_templates.hpp"

namespace ui {
	void message_log_text::on_update(sys::state& state) noexcept {
		int32_t index = retrieve<int32_t>(state, parent);
		auto const& messages = static_cast<ui::message_log_window*>(state.ui_state.msg_log_window)->messages;
		if(index < int32_t(messages.size())) {
			auto m = messages[index];
			auto container = text::create_endless_layout(state, internal_layout,
			text::layout_parameters{0, 0, base_data.size.x, base_data.size.y, base_data.data.button.font_handle, 0,
			text::alignment::left, text::text_color::white, false});

			auto box = text::open_layout_box(container);
			text::add_to_layout_box(state, container, box, text::embedded_flag{ state.world.nation_get_identity_from_identity_holder(m.source) });
			text::add_space_to_layout_box(state, container, box);
			if(m.target) {
				text::add_to_layout_box(state, container, box, text::embedded_flag{ state.world.nation_get_identity_from_identity_holder(m.target) });
				text::add_space_to_layout_box(state, container, box);
			}
			text::add_to_layout_box(state, container, box, std::string_view{ ":" });
			text::add_space_to_layout_box(state, container, box);
			text::localised_format_box(state, container, box, m.title);
			text::close_layout_box(container, box);
		}
	}

	void message_log_text::update_tooltip(sys::state& state, int32_t x, int32_t y, text::columnar_layout& contents) noexcept {
		int32_t index = retrieve<int32_t>(state, parent);
		auto const& messages = static_cast<ui::message_log_window*>(state.ui_state.msg_log_window)->messages;
		if(index < int32_t(messages.size())) {
			messages[index].body(state, contents);
		}
	}

	void message_log_window::on_create(sys::state& state) noexcept {
		window_element_base::on_create(state);
		cat_filters.resize(size_t(message_settings_category::count));
	}

	std::unique_ptr<element_base> message_log_window::make_child(sys::state& state, std::string_view name, dcon::gui_def_id id) noexcept {
		if(name == "close" || name == "closebutton") {
			return make_element_by_type<message_log_close_button>(state, id);
		} else if(name == "messagelog") {
			auto ptr = make_element_by_type<message_log_listbox>(state, id);
			log_list = ptr.get();
			return ptr;
		} else if(name == "messagecat_combat") {
			return make_element_by_type<message_log_filter_checkbox<message_settings_category::combat>>(state, id);
		} else if(name == "messagecat_diplomacy") {
			return make_element_by_type<message_log_filter_checkbox<message_settings_category::diplomacy>>(state, id);
		} else if(name == "messagecat_units") {
			return make_element_by_type<message_log_filter_checkbox<message_settings_category::units>>(state, id);
		} else if(name == "messagecat_provinces") {
			return make_element_by_type<message_log_filter_checkbox<message_settings_category::provinces>>(state, id);
		} else if(name == "messagecat_events") {
			return make_element_by_type<message_log_filter_checkbox<message_settings_category::events>>(state, id);
		} else if(name == "messagecat_others") {
			return make_element_by_type<message_log_filter_checkbox<message_settings_category::others>>(state, id);
		} else {
			return nullptr;
		}
	}

	void message_log_window::on_update(sys::state& state) noexcept {

		if(auto ex_size = int32_t(messages.size()) - 100; ex_size > 0) {
			std::rotate(messages.begin(), messages.begin() + ex_size, messages.end());
			messages.resize(100);
		}

		bool at_bottom = log_list->list_scrollbar->raw_value() == log_list->list_scrollbar->settings.upper_limit;

		log_list->row_contents.clear();
		for(int32_t i = 0; i < int32_t(messages.size()); ++i)
		log_list->row_contents.push_back(i);

		log_list->update(state);

		if(at_bottom)
		log_list->scroll_to_bottom(state);
	}

	message_result message_log_window::get(sys::state& state, Cyto::Any& payload) noexcept {
		if(payload.holds_type<message_settings_category>()) {
			auto cat = any_cast<message_settings_category>(payload);
			payload.emplace<bool>(cat_filters[uint8_t(cat)]);
			return message_result::consumed;
		} else if(payload.holds_type<element_selection_wrapper<message_settings_category>>()) {
			auto cat = any_cast<element_selection_wrapper<message_settings_category>>(payload).data;
			cat_filters[uint8_t(cat)] = !cat_filters[uint8_t(cat)];
			impl_on_update(state);
			return message_result::consumed;
		}
		return message_result::unseen;
	}
} // namespace ui
