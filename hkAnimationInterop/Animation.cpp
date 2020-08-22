#include "stdafx.h"

#include <iostream>

#include "Animation.h"
#include "AnimationContainer.h"
#include "Skeleton.h"

#include <Animation/Animation/hkaAnimation.h>
#include <Animation/Animation/Playback/hkaAnimatedSkeleton.h>
#include <Animation/Animation/Playback/Control/Default/hkaDefaultAnimationControl.h>
#include <Animation/Animation/Rig/hkaPose.h>
#include <Animation/Animation/Rig/hkaSkeletonUtils.h>

Animation::Animation(AnimationContainer* container, hkaAnimationBinding* binding)
{
    this->m_Container = container;
    this->m_Binding = binding;
    this->m_Animation = binding->m_animation;

    this->m_AnimationControl = new hkaDefaultAnimationControl(binding);
    this->m_AnimatedSkeleton = new hkaAnimatedSkeleton(container->getSkeleton()->getHavokSkeleton());
    this->m_AnimatedSkeleton->addAnimationControl(this->m_AnimationControl);
}


Animation::~Animation()
{
}

const float Animation::getDuration() {
    return this->m_Animation->m_duration;
}
const int Animation::getNumFrames() {
    return this->m_Animation->getNumOriginalFrames();
}

Transform* Animation::getPose(const float time, int &count)
{
    this->m_AnimationControl->setLocalTime(time);

    hkaPose pose(this->m_AnimatedSkeleton->getSkeleton());
    this->m_AnimatedSkeleton->sampleAndCombineAnimations(pose.accessUnsyncedPoseLocalSpace().begin(), pose.getFloatSlotValues().begin());

    const hkQsTransform *transforms = pose.getSyncedPoseLocalSpace().begin();
    
    count = this->m_AnimatedSkeleton->getSkeleton()->m_bones.getSize();
    Transform *result = new Transform[count];
    for (int i = 0; i < count; ++i) {
        hkQsTransform currentIn = transforms[i];
        hkVector4f translation = currentIn.getTranslation();
        hkVector4f scale = currentIn.getScale();
        hkVector4f rotation = currentIn.getRotation().m_vec;
        
        Transform currentOut;

        currentOut.Translation.X = translation(0);
        currentOut.Translation.Y = translation(1);
        currentOut.Translation.Z = translation(2);
        currentOut.Translation.W = translation(3);

        currentOut.Scale.X = scale(0);
        currentOut.Scale.Y = scale(1);
        currentOut.Scale.Z = scale(2);
        currentOut.Scale.W = scale(3);

        currentOut.Rotation.X = rotation(0);
        currentOut.Rotation.Y = rotation(1);
        currentOut.Rotation.Z = rotation(2);
        currentOut.Rotation.W = rotation(3);

        result[i] = currentOut;
    }

    return result;
}


