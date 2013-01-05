/*
    Copyright © 2010, 2011, 2012 Vladimír Vondruš <mosra@centrum.cz>

    This file is part of Magnum.

    Magnum is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License version 3
    only, as published by the Free Software Foundation.

    Magnum is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU Lesser General Public License version 3 for more details.
*/

#include "Profiler.h"

#include <algorithm>
#include <numeric>
#include <Utility/Assert.h>

#include "Magnum.h"

namespace Magnum { namespace DebugTools {

Profiler::Section Profiler::addSection(const std::string& name) {
    CORRADE_ASSERT(!enabled, "Profiler: cannot add section when profiling is enabled", 0);
    sections.push_back(name);
    return sections.size()-1;
}

void Profiler::setMeasureDuration(std::size_t frames) {
    CORRADE_ASSERT(!enabled, "Profiler: cannot set measure duration when profiling is enabled", );
    measureDuration = frames;
}

void Profiler::enable() {
    enabled = true;
    frameData.assign(measureDuration*sections.size(), std::chrono::high_resolution_clock::duration::zero());
    totalData.assign(sections.size(), std::chrono::high_resolution_clock::duration::zero());
    frameCount = 0;
}

void Profiler::disable() {
    enabled = false;
}

void Profiler::start(Section section) {
    if(!enabled) return;
    CORRADE_ASSERT(section < sections.size(), "Profiler: unknown section passed to start()", );

    save();

    currentSection = section;
}

void Profiler::stop() {
    if(!enabled) return;

    save();

    previousTime = std::chrono::high_resolution_clock::time_point();
}

void Profiler::save() {
    auto now = std::chrono::high_resolution_clock::now();

    /* If the profiler is already running, add time to given section */
    if(previousTime != std::chrono::high_resolution_clock::time_point())
        frameData[currentFrame*sections.size()+currentSection] += now-previousTime;

    /* Set current time as previous for next section */
    previousTime = now;
}

void Profiler::nextFrame() {
    if(!enabled) return;

    /* Next frame index */
    std::size_t nextFrame = (currentFrame+1) % measureDuration;

    /* Add times of current frame to total */
    for(std::size_t i = 0; i != sections.size(); ++i)
        totalData[i] += frameData[currentFrame*sections.size()+i];

    /* Subtract times of next frame from total and erase them */
    for(std::size_t i = 0; i != sections.size(); ++i) {
        totalData[i] -= frameData[nextFrame*sections.size()+i];
        frameData[nextFrame*sections.size()+i] = std::chrono::high_resolution_clock::duration::zero();
    }

    /* Advance to next frame */
    currentFrame = nextFrame;

    if(frameCount < measureDuration) ++frameCount;
}

void Profiler::printStatistics() {
    if(!enabled) return;

    std::vector<std::size_t> totalSorted(sections.size());
    std::iota(totalSorted.begin(), totalSorted.end(), 0);

    std::sort(totalSorted.begin(), totalSorted.end(), [this](std::size_t i, std::size_t j){return totalData[i] > totalData[j];});

    Debug() << "Statistics for last" << measureDuration << "frames:";
    for(std::size_t i = 0; i != sections.size(); ++i)
        Debug() << ' ' << sections[totalSorted[i]] << std::chrono::microseconds(totalData[totalSorted[i]]).count()/frameCount << u8"µs";
}

}}