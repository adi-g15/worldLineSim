#include "Entities/Meta/Custom/GDSC/gdsc.hpp"
#include "entity.hpp"
#include "entity_types.hpp"
#include "logger.hpp"
#include "world.hpp"
#include <algorithm>
#include <cassert>
#include <fstream>

#include <iterator>
#include <math.h>
#include <string>
#include <util/random.hpp>

void Member::simulateExistence() {
    return;
    /*members remain with GDSC for now, abhi ke liye pure GDSC ko ghumne padega
     * sorry :) */
    this->has_been_paused = false;

    while (this->should_wander && this->parent_world->is_world_running()) {
        LOGGER::log_msg("[{} #{}] Refreshing factors...", mName, this->_id,
                        this->curr_pos.point_coord);

        this->refresh_metric();

        // Thoda chal lete hai...
        graphMat::NeighbourIterator<Box> iter(this->curr_pos.graph_box);

        auto random_number_of_loops = rand() % 4;
        do {
            ++iter; // ie. skip the current box
        } while (random_number_of_loops-- > 0);

        // for (; false /*one time, remove block*/ ; ++iter)
        // {
        // 	if ( ! iter->getData().hasEntities()) {
        // 		this->curr_pos.graph_box->getDataRef().remEntity(this);

        // 		coord increment_coord = iter._getIncrementCoords();

        // 		if (curr_pos.graph_box != iter.center_box) {
        // 			// if it's not the center box we passed to
        // initialiser, then it is EITHER FRONT_FACING or BACK_FACING
        // 			increment_coord.mZ = (curr_pos.graph_box->FRONT_FACING
        // == iter.center_box) ? 1 : -1;
        // 		}
        // 		this->curr_pos.point_coord += increment_coord;

        // 		this->curr_pos.graph_box->getDataRef().addEntity(this);

        // 		break;
        // 	}
        // }

        std::this_thread::sleep_for(statics::UNIT_TIME * 2);
    }

    this->has_been_paused = true;
}

void Member::pauseExistence() {
    /*jab chala hi nhi to roke kyu :) */

    if (!should_wander)
        return;

    this->should_wander = false;
    while (!this->has_been_paused) {
    } // blocking wait.... till it's stopped
}

// metrics === factors
void Member::refresh_metric() {
    auto &p = this->interactions[last_msg.substr(0, 3)];
    p.second = p.second * fmod((-0.2 + rand()), 1) * p.first;

    // p.first ko yaha change nhi krenge, it's changed in interaction with mouth
    // or ears
}

void Member::ears_listen(std::string sounds) {
    LOGGER::log_msg("[GDSC {}] heard... \"{}\"", mName, sounds /*.substr(4)*/);

    this->last_msg = sounds;
    // TODO: Basic ettiquete h yar, reply... implement it

    // we had another interaction, update it... eventually we will refresh
    // metrics
    this->interactions[sounds.substr(0, 3)].first += sounds.size();
}

void Member::mouth_speak() {
    auto message_bytes =
        util::Random::random_bytes(ceil(this->talkative_factor * 15));

    if (rand() % 10 == 1) { // a 10% chance
        LOGGER::log_msg("[{}]: Hello Hello ! Aawaj aa rahi hai ? 🥲", mName);
    }

    auto message = std::string(reinterpret_cast<char *>(message_bytes.data()),
                               reinterpret_cast<char *>(message_bytes.data()) +
                                   message_bytes.size());
    auto receiver = m_chose_member();

    message.push_back('\0');
    LOGGER::log_msg("[{} #{}]: {}", mName, _id, message);

    this->gdsc->broadcast(receiver + message);
}

std::string Member::m_chose_member() {
    auto members = this->gdsc->get_members_ref();

    auto iter = members.cbegin();
    // auto index = util::Random::random(members.size());
    std::advance(iter, util::Random::random() % members.size());

    return iter->first;
}

GDSC::GDSC(World_Ptr const world, const std::string &full_name,
           const std::map<std::string, Member> &members)
    : Entity(Entity_Types::META), parent_world(world), group(members),
      full_name(full_name) {
    for (auto &m : group) {
        m.second.gdsc = this;
    }
    assert(members.size() != 0); /* A GDSC MUST HAVE SOME MEMBER*/
    LOGGER::log_msg("[GDSC #{}] {} established", _id, full_name);
}

GDSC::GDSC(World_Ptr const world, const std::string &full_name,
           const std::string &members_details_file)
    : GDSC(world, full_name, _get_members_from_file(members_details_file)) {}

std::map<std::string, Member>
GDSC::_get_members_from_file(const std::string &members_details_file) {
    // TODO: Read file, create std::map then call the other constructor
    //
    return {};
}

const std::map<std::string, Member> &GDSC::get_members_ref() const {
    return group;
}

void GDSC::broadcast(std::string msg) {
    auto prefix = msg.substr(0, 3);

    auto receivers = std::vector<std::string>();
    // Ab hum wildcards use karenge
    auto wildcard_pos = msg.find_first_of("*");
    if (wildcard_pos != std::string::npos) {
        msg.erase(wildcard_pos);
    }
    for (const auto &p : group) {
        // https://stackoverflow.com/questions/1878001/how-do-i-check-if-a-c-stdstring-starts-with-a-certain-string-and-convert-a
        if (p.first.rfind(prefix, 0) == 0) {
            receivers.push_back(p.first);
        }
    }

    if (receivers.size() > 1) {
        LOGGER::log_msg("[GDSC #{}] Kaafi discussion ho raha hai :D...", _id);
    }

    for (const auto &r : receivers) {
        this->group.at(r).ears_listen(msg);
    }
}

void GDSC::simulateExistence() {
    pause_requested = false;
    while (!pause_requested) {
        auto people_in_discussion = rand() % 2 + 1;

        while (people_in_discussion-- > 0) {
            auto iter = group.begin();
            std::advance(iter, rand() % group.size());

            iter->second.mouth_speak();
        }

        std::this_thread::sleep_for(statics::UNIT_TIME * 0.6);
    }

    pause_acknowledged = true;
}

void GDSC::pauseExistence() {
    pause_requested = false;
    LOGGER::log_msg("[GDSC {} #{}]: Phir milenge...", this->full_name,
                    this->_id);

    // blocking wait till paused
    while (!pause_acknowledged) {
    }
}
