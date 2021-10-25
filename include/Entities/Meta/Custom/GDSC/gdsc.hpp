#pragma once

#include "Entities/human.hpp"
#include "entity.hpp"
#include "entity_types.hpp"
#include "world.hpp"
#include <map>
#include <optional>
#include <string>

using SamajdharHuman =
    Human; // @adi Hata dena baad me, jab Humans intelligent ho

class GDSC;

class Member : SamajdharHuman {
    std::string role;
    float talkative_factor;
    float listening_factor;
    std::string last_msg;
    std::map<std::string, std::pair<uint64_t, float>>
        interactions; // {"name" : {"total talk (bytes)", "inference score"} }

    GDSC * /*const*/ gdsc;

    std::string m_chose_member();

  public:
    // @adi Future C:2
    void mouth_speak(/*const std::string prefix_name*/);
    void refresh_metric();
    void ears_listen(std::string sounds);
    Member(World_Ptr const world, const std::string &name, Gender gender,
           const std::string &role = "Member", float talkative_factor = 0.5,
           float listening_factor = 0.6)
        : role(role),
          // gdsc(gdsc),
          talkative_factor(talkative_factor),
          listening_factor(listening_factor), Human(world, name, gender) {}

    // Overloads
    void _Action1() override {}
    void _Action2() override {}

    void simulateExistence() override;
    void pauseExistence() override;

    friend class GDSC;
};

class GDSC : public Entity {
    std::string full_name;
    std::map<std::string, Member> group;
    bool pause_requested{false}, pause_acknowledged{false};
    World_Ptr const parent_world;
    std::map<std::string, Member>
    _get_members_from_file(const std::string &filepath);

  public:
    void broadcast(std::string msg);
    const std::map<std::string, Member> &get_members_ref() const;
    GDSC(World_Ptr const world, const std::string &full_name,
         const std::map<std::string, Member> &members);
    GDSC(World_Ptr const world, const std::string &full_name,
         const std::string &member_details_yaml);

    // Overloads
    void _Action1() override {}
    void _Action2() override {}
    std::optional<Entity_Point> getPrimaryPos() const override {
        return std::nullopt;
    }
    EntityState *_get_current_state() const {
        return new EntityState(Entity_Types::META);
    }

    void simulateExistence() override;
    void pauseExistence() override;
};
