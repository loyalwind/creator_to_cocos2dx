
#include "AnimationManager.h"
#include "AnimateClip.h"

NS_CCR_BEGIN

AnimationManager::AnimationManager()
{}

AnimationManager::~AnimationManager()
{
    for (auto&& animationInfo : _animations)
        animationInfo.target->release();
	stopAllAnimationClips();
}

void AnimationManager::cleanup()
{
	Node::cleanup();
	stopAllAnimationClips();
}

void AnimationManager::addAnimation(const AnimationInfo& animationInfo)
{
    _animations.push_back(animationInfo);
}

void AnimationManager::playOnLoad()
{
    for (auto& animationInfo : _animations)
    {
        if (animationInfo.playOnLoad && animationInfo.defaultClip)
            runAnimationClip(animationInfo.target, animationInfo.defaultClip,nullptr);
    }
}

void AnimationManager::stopAnimationClipsRunByPlayOnLoad()
{
    for (auto& animationInfo : _animations)
    {
        if (animationInfo.playOnLoad && animationInfo.defaultClip)
            stopAnimationClip(animationInfo.target, animationInfo.defaultClip->getName());
    }
}

void AnimationManager::stopAllAnimationClips()
{
	if (!_cachedAnimates.empty())
	{
		for (auto iter = _cachedAnimates.begin(), end = _cachedAnimates.end(); iter != end; ++iter)
		{
			auto&& e = *iter;
			// release AnimateClip
			std::get<2>(e)->pauseAnimate();
			std::get<2>(e)->autorelease();
		}
	}
	
	_cachedAnimates.clear();
}


void AnimationManager::playAnimationClip(cocos2d::Node *target, const std::string &animationClipName, AnimationCallback cb, int modeType)
{
    bool foundTarget = false;
    bool foundAnimationClip = false;
    
    for (auto& animationInfo : _animations)
    {
       // if (animationInfo.target == target)
        {
            for (auto& animClip : animationInfo.clips)
            {
                if (animClip->getName() == animationClipName)
                {
					if (modeType != -1)
					{
						animClip->setWrapMode((AnimationClip::WrapMode)modeType);
					}
                    runAnimationClip(target, animClip, cb);
                    foundAnimationClip = true;
					foundTarget = true;
                    break;
                }
            }
            
			if (foundTarget)
			{
				break;
			}
           // break;
        }
    }
    
    if (!foundTarget)
        CCLOG("can not found target: %p", target);
    else
    {
        if (!foundAnimationClip)
            CCLOG("can not found animation clip name %s of target %p", animationClipName.c_str(), target);
    }
}

void AnimationManager::stopAnimationClip(cocos2d::Node *target, const std::string &animationClipName)
{
    auto animateClip = getAnimateClip(target, animationClipName);
    if (animateClip)
    {
        animateClip->stopAnimate();
        removeAnimateClip(target, animationClipName);
    }
}

void AnimationManager::pauseAnimationClip(cocos2d::Node *target, const std::string &animationClipName)
{
    auto animateClip = getAnimateClip(target, animationClipName);
    if (animateClip)
        animateClip->pauseAnimate();
}

void AnimationManager::resumeAnimationClip(cocos2d::Node *target, const std::string &animationClipName)
{
    auto animateClip = getAnimateClip(target, animationClipName);
    if (animateClip)
        animateClip->resumeAnimate();
}

void AnimationManager::runAnimationClip(cocos2d::Node *target, AnimationClip* animationClip,AnimationCallback cb)
{
    auto animate = AnimateClip::createWithAnimationClip(target, animationClip);
    animate->retain();
    this->retain();
    animate->setCallbackForEndevent([=]() {
		if (cb){
			cb();
		}
        removeAnimateClip(target, animationClip->getName());
        this->release();
    });
    
    animate->startAnimate();
    _cachedAnimates.push_back(std::make_tuple(target, animationClip->getName(), animate));
}

void AnimationManager::removeAnimateClip(cocos2d::Node *target, const std::string &animationClipName)
{
	if (!_cachedAnimates.empty())
	{
		for (auto iter = _cachedAnimates.begin(), end = _cachedAnimates.end(); iter != end; ++iter)
		{
			auto&& e = *iter;
			if (std::get<0>(e) == target && std::get<1>(e) == animationClipName)
			{
				// release AnimateClip
				std::get<2>(e)->autorelease();

				_cachedAnimates.erase(iter);
				break;
			}
		}
	}
}

AnimateClip* AnimationManager::getAnimateClip(cocos2d::Node *target, const std::string &animationClipName) const
{
    for (auto&& e : _cachedAnimates)
    {
        if (std::get<0>(e) == target && std::get<1>(e) == animationClipName)
            return std::get<2>(e);
    }
    
    return nullptr;
}

NS_CCR_END
