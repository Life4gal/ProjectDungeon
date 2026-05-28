// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

namespace pd::component::state
{
	// 处于相机(可视)区域内
	// class InCameraArea {};

	// 响应相机更新
	// class CameraAware final {};
	// 不响应相机更新
	class CameraIgnore final {};

	// ====================================
	// 实体
	// ====================================

	namespace entity
	{
		// 实体应当被销毁
		class Dead final {};
	}

	// ====================================
	// SPRITE
	// ====================================

	namespace sprite
	{
		// 由InCameraArea拆分而来
		// O -> 参与更新&渲染
		// X -> 不参与更新&渲染
		class Awake {};

		// 精灵不可见
		// O -> 不参与渲染
		// X -> 参与渲染
		class Invisible final {};
	}

	// ====================================
	// PROPERTY
	// ====================================

	namespace property
	{
		// 由InCameraArea拆分而来
		// O -> 参与更新
		// X -> 不参与更新
		class Awake {};

		// // 濒死
		// // O -> 不参与更新&渲染
		// // X -> 参与更新&渲染
		// // TODO: 并不准确?如果我们会有死亡动画呢?动画不属于原实体?
		// class Dying final {};
	}

	// ====================================
	// AI
	// ====================================

	namespace ai
	{
		// 由InCameraArea拆分而来
		// O -> 参与更新
		// X -> 不参与更新
		class Awake {};
	}
}
