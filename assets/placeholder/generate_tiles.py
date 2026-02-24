# 生成占位符瓦片纹理
# 运行: python generate_tiles.py
# 需要: pip install pillow

from PIL import Image, ImageDraw

# 图片尺寸: 256x192 (4x3 个 64x64 瓦片)
WIDTH = 256
HEIGHT = 192
TILE_SIZE = 64

# 颜色定义 (R, G, B)
COLORS = {
    # 第一行 (y=0)
    'floor_stone': (60, 60, 60),       # 深灰色地板
    'wall_stone': (100, 80, 60),       # 棕色墙壁
    'wall_corner_tl': (120, 100, 80),  # 左上角墙壁
    'wall_corner_tr': (120, 100, 80),  # 右上角墙壁
    
    # 第二行 (y=64)
    'door_closed': (139, 69, 19),      # 棕红色关闭门
    'door_open': (50, 50, 50),         # 深灰色打开门
    'wall_corner_bl': (120, 100, 80),  # 左下角墙壁
    'wall_corner_br': (120, 100, 80),  # 右下角墙壁
    
    # 第三行 (y=128)
    'floor_decoration': (80, 80, 60),  # 地板装饰
}

def draw_tile(draw, x, y, color, label=None):
    """绘制一个瓦片"""
    # 填充颜色
    draw.rectangle([x, y, x + TILE_SIZE - 1, y + TILE_SIZE - 1], fill=color)
    
    # 添加边框
    border_color = tuple(max(0, c - 30) for c in color)
    draw.rectangle([x, y, x + TILE_SIZE - 1, y + TILE_SIZE - 1], outline=border_color, width=2)
    
    # 添加简单的内部图案区分不同瓦片
    inner_color = tuple(min(255, c + 20) for c in color)
    draw.rectangle([x + 8, y + 8, x + TILE_SIZE - 9, y + TILE_SIZE - 9], outline=inner_color, width=1)

def draw_corner(draw, x, y, color, corner_type):
    """绘制角落瓦片"""
    draw.rectangle([x, y, x + TILE_SIZE - 1, y + TILE_SIZE - 1], fill=color)
    
    # 根据角落类型绘制L形
    dark = tuple(max(0, c - 40) for c in color)
    light = tuple(min(255, c + 30) for c in color)
    
    if corner_type == 'tl':  # 左上
        draw.rectangle([x, y, x + 10, y + TILE_SIZE - 1], fill=dark)
        draw.rectangle([x, y, x + TILE_SIZE - 1, y + 10], fill=dark)
    elif corner_type == 'tr':  # 右上
        draw.rectangle([x + TILE_SIZE - 11, y, x + TILE_SIZE - 1, y + TILE_SIZE - 1], fill=dark)
        draw.rectangle([x, y, x + TILE_SIZE - 1, y + 10], fill=dark)
    elif corner_type == 'bl':  # 左下
        draw.rectangle([x, y, x + 10, y + TILE_SIZE - 1], fill=dark)
        draw.rectangle([x, y + TILE_SIZE - 11, x + TILE_SIZE - 1, y + TILE_SIZE - 1], fill=dark)
    elif corner_type == 'br':  # 右下
        draw.rectangle([x + TILE_SIZE - 11, y, x + TILE_SIZE - 1, y + TILE_SIZE - 1], fill=dark)
        draw.rectangle([x, y + TILE_SIZE - 11, x + TILE_SIZE - 1, y + TILE_SIZE - 1], fill=dark)

def draw_door(draw, x, y, is_open):
    """绘制门瓦片"""
    if is_open:
        # 打开的门 - 深色通道
        draw.rectangle([x, y, x + TILE_SIZE - 1, y + TILE_SIZE - 1], fill=(30, 30, 30))
        # 门框
        draw.rectangle([x, y, x + 8, y + TILE_SIZE - 1], fill=(100, 70, 40))
        draw.rectangle([x + TILE_SIZE - 9, y, x + TILE_SIZE - 1, y + TILE_SIZE - 1], fill=(100, 70, 40))
    else:
        # 关闭的门 - 木门
        draw.rectangle([x, y, x + TILE_SIZE - 1, y + TILE_SIZE - 1], fill=(139, 69, 19))
        # 门板纹理
        draw.rectangle([x + 4, y + 4, x + 28, y + TILE_SIZE - 5], fill=(120, 60, 20))
        draw.rectangle([x + 34, y + 4, x + TILE_SIZE - 5, y + TILE_SIZE - 5], fill=(120, 60, 20))
        # 门把手
        draw.ellipse([x + TILE_SIZE - 18, y + 28, x + TILE_SIZE - 10, y + 36], fill=(180, 150, 50))

def main():
    # 创建图片
    img = Image.new('RGBA', (WIDTH, HEIGHT), (0, 0, 0, 0))
    draw = ImageDraw.Draw(img)
    
    # 第一行
    draw_tile(draw, 0, 0, COLORS['floor_stone'])      # floor_stone
    draw_tile(draw, 64, 0, COLORS['wall_stone'])      # wall_stone
    draw_corner(draw, 128, 0, COLORS['wall_corner_tl'], 'tl')  # wall_corner_tl
    draw_corner(draw, 192, 0, COLORS['wall_corner_tr'], 'tr')  # wall_corner_tr
    
    # 第二行
    draw_door(draw, 0, 64, False)   # door_closed
    draw_door(draw, 64, 64, True)   # door_open
    draw_corner(draw, 128, 64, COLORS['wall_corner_bl'], 'bl')  # wall_corner_bl
    draw_corner(draw, 192, 64, COLORS['wall_corner_br'], 'br')  # wall_corner_br
    
    # 第三行
    draw_tile(draw, 0, 128, COLORS['floor_decoration'])  # floor_decoration
    
    # 保存
    img.save('tiles.png')
    print('已生成 tiles.png (256x192)')

if __name__ == '__main__':
    main()
