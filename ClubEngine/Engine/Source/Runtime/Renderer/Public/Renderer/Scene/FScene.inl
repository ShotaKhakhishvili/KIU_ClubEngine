#pragma once

namespace CE
{

template<typename F>
void FScene::ForEachPrimitive(F&& func) const
{
    primitiveProxyPool.ForEach(std::forward<F>(func));
}

template<typename F>
void FScene::ForEachLight(F&& func) const
{
    lightProxyPool.ForEach(std::forward<F>(func));
}

}