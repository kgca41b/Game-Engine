#include "SoundSystem.h"
#include <io.h>

using namespace KGCA41B;


void SoundSystem::OnCreate(entt::registry& reg)
{
    sound_directory_address_ = SOUND_PATH;

    if (sound_directory_address_.empty())
        return;

    FMOD_RESULT hr = FMOD::System_Create(&fmod_system_);
    fmod_system_->init(32, FMOD_INIT_NORMAL, nullptr);

    LoadDir(sound_directory_address_);

    MakeSoundPool();
}

void SoundSystem::OnUpdate(entt::registry& reg)
{
    CheckGenerators(reg);

    CheckPlayingPool();

    fmod_system_->update();
}

bool SoundSystem::OnRelease()
{
    return false;
}

void SoundSystem::CheckGenerators(entt::registry& reg)
{
    // ������Ʈ������ ���� ���ʷ����� ������Ʈ�� ������ �ִ� ��ƼƼ���� ���� ť�� ������ ������ ���带 ����Ѵ�.
    auto generators = reg.view<SoundGenerator>();
    for (auto entity : generators)
    {
        auto& generator = generators.get<SoundGenerator>(entity);
        while (generator.sound_queue_list.size() != 0)
        {
            auto queue = generator.sound_queue_list.front();

            // ������ ������Ʈ�� ������ �ִ� ��ƼƼ�鿡 ���� 3D ���� ��� �� ���� ����
            auto listeners = reg.view<SoundListener>();
            for (auto entity2 : listeners)
            {
                auto& generator_transform = reg.get<Transform>(entity);
                auto& listener_transform = reg.get<Transform>(entity2);
                // TODO : Transform ���� �� ��ĥ ����
                XMVECTOR genertor_position = XMVectorSet(generator_transform.world_matrix.r[3].m128_f32[0], generator_transform.world_matrix.r[3].m128_f32[1],
                    generator_transform.world_matrix.r[3].m128_f32[2], 0);
                XMVECTOR listener_position = XMVectorSet(listener_transform.world_matrix.r[3].m128_f32[0], listener_transform.world_matrix.r[3].m128_f32[1],
                    listener_transform.world_matrix.r[3].m128_f32[2], 0);
                XMVECTOR pos = genertor_position - listener_position;
                Play(queue.sound_filename, queue.is_looping, queue.sound_volume, pos);
            }

            // ����ߴٸ� ť���� ����
            generator.sound_queue_list.pop();
        }
    }
}

void SoundSystem::CheckPlayingPool()
{
    for (auto iter = sound_pool_play.begin(); iter != sound_pool_play.end(); )
    {
        SoundData* sound = *iter;
        // ��� ���� ������ ���� �ð� ����
        sound->channel->getPosition(&sound->current_time, FMOD_TIMEUNIT_MS);

        // ���ŵ� ���尡 �����ٸ� �ٽ� Ǯ�� �ֱ�, FMOD::Sound�� �ٽ� Play�ϸ� ����ȴ�.
        if (sound->current_time >= sound->total_time)
        {
            sound_pool_wait[sound->sound_filename].push(sound);

            iter = sound_pool_play.erase(iter);
            if (iter == sound_pool_play.end())
                break;
        }
        iter++;
    }
}

void SoundSystem::Play(wstring sound_name, bool looping, float volume, FXMVECTOR generate_pos)
{
    if (sound_pool_wait.find(sound_name) == sound_pool_wait.end())
        return;


    // TODO : 3DAttrubutes ���� Velocity �� ���� �ʿ�, 3DLevel �� ���� �ʿ�
    FMOD_VECTOR pos = { generate_pos.m128_f32[0], generate_pos.m128_f32[1], generate_pos.m128_f32[2] };

    // TODO : ���� Ǯ�� �̿��� ���� ������ ��������
    SoundData* sound_data = sound_pool_wait[sound_name].front();
    sound_pool_wait[sound_name].pop();

    sound_data->sound->setMode(looping ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF);
    sound_data->channel->setVolume(volume);

    sound_data->sound->set3DMinMaxDistance(0, 300);
    sound_data->channel->set3DAttributes(&pos, &pos);
    sound_data->channel->set3DLevel(6);

    fmod_system_->playSound(sound_data->sound, nullptr, false, &sound_data->channel);

    sound_pool_play.push_back(sound_data);
}

void SoundSystem::LoadDir(wstring directory_address)
{
    std::wstring wholeAddr = directory_address + L"*.*";
    intptr_t handle;
    struct _wfinddata_t fd;
    handle = _wfindfirst(wholeAddr.c_str(), &fd);

    // ��ã���� ����
    if (handle == -1L) return;

    do {
        if ((fd.attrib & _A_SUBDIR) && (fd.name[0] != '.'))
        {
            LoadDir(directory_address + fd.name + L"/");
        }
        else if (fd.name[0] != '.')
        {
            LoadFile(directory_address + fd.name);
            
        }
    } while (_wfindnext(handle, &fd) == 0);
}

void SoundSystem::LoadFile(wstring file_address)
{
    SoundData* newSound = new SoundData;

    FMOD_RESULT hr = fmod_system_->createSound(to_wm(file_address).c_str(), (FMOD_MODE)(FMOD_3D), nullptr, &newSound->sound);
    if (hr == FMOD_OK)
    {
        newSound->sound_filename = file_address;
        newSound->sound->getLength(&newSound->total_time, FMOD_TIMEUNIT_MS);
    }
    sound_resource_list_.insert({ file_address, newSound});
}

void SoundSystem::MakeSoundPool()
{
    for (auto& resource : sound_resource_list_)
    {
        for (int i = 0; i < POOL_SIZE; i++)
        {
            sound_pool_wait[resource.first].push(resource.second);
        }
            
    }
}
