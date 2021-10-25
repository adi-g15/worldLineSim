#include "Entities/Meta/Bhakt/neeraj-watcher.hpp"
#include "Entities/Custom/loki.hpp"
#include "logger.hpp"

id_type NeerajWatcher::get_worldline_id() const {
    return this->worldline_id();
}

void NeerajWatcher::shift_worldline(Entity* entity) /*can throw...*/ {
    this->world_node->diverge_without_change();

    auto num_nodes = this->world_node->diverged_child.size();
    if( num_nodes ) { LOGGER::log_msg("ERROR: NO NODES AFTER DIVERGING"); }
    
    auto loki = reinterpret_cast<Loki*>(entity);
    auto& previous_worldline_entities = parent_world->entities;
    auto& next_worldline_entities = this->get_entities_vec();
    
    next_worldline_entities.push_back(loki);

    this->diverged_child[ rand() % num_nodes ]
}
