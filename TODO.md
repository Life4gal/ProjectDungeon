# TODO LIST

- [ ] component/tags.hpp 增加 state::DirtyRenderer 用于标记那些在此帧有改动的实体,这应该可以简化收集渲染单元的过程,避免更新Batch,仅提交修改的部分draw.
