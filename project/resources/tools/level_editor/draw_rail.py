import bpy
import gpu
import gpu_extras.batch
import mathutils

class DrawRail:
    handle = None
    
    # 描画用データキャッシュ (毎回計算すると重いため、本来は更新時のみ計算すべきだが、
    # 簡略化のためdraw_callback内で計算する。あるいはdepsgraph_updateを使う手もある)
    
    def catmull_rom_interpolation(p0, p1, p2, p3, t):
        s = 0.5
        t2 = t * t
        t3 = t2 * t
        e3 = p0 * -1.0 + p1 * 3.0 - p2 * 3.0 + p3
        e2 = p0 * 2.0 - p1 * 5.0 + p2 * 4.0 - p3
        e1 = p0 * -1.0 + p2
        e0 = p1 * 2.0
        return (e3 * t3 + e2 * t2 + e1 * t + e0) * s

    def catmull_rom_position(points, t):
        n = len(points)
        if n < 2:
             return points[0] # エラー回避
        if n == 2:
            return points[0].lerp(points[1], t)
        division = n - 1
        area_width = 1.0 / division
        index = int(t / area_width)
        if index >= division:
            index = division - 1
        t0 = area_width * index
        t_2 = (t - t0) / area_width
        t_2 = max(0.0, min(1.0, t_2))
        index0 = max(0, index - 1)
        index1 = index
        index2 = min(n - 1, index + 1)
        index3 = min(n - 1, index + 2)
        p0 = points[index0]
        p1 = points[index1]
        p2 = points[index2]
        p3 = points[index3]
        return DrawRail.catmull_rom_interpolation(p0, p1, p2, p3, t_2)

    def draw_rail_spline(rail_obj):
        # 子供のControlPointSpawnを取得
        control_points = []
        # 名前順にソートして取得 (Blenderのアウトライナー順が望ましいが、簡易的に名前順)
        # childrenは順不同なので注意が必要
        children = [child for child in rail_obj.children if child.get("type") == "ControlPointSpawn"]
        children.sort(key=lambda x: x.name)
        
        for child in children:
            control_points.append(child.matrix_world.translation.copy())
            
        if len(control_points) < 2:
            return

        points_drawing = []
        segment_count = 100 * (len(control_points) - 1) # 点の数に応じてセグメント数を増やす
        
        for i in range(segment_count + 1):
            t = 1.0 / segment_count * i
            pos = DrawRail.catmull_rom_position(control_points, t)
            points_drawing.append(pos)
            
        if not points_drawing:
            return

        vertices = {"pos": [v for v in points_drawing]}
        indices = [[i, i + 1] for i in range(len(points_drawing) - 1)]
        shader = gpu.shader.from_builtin("UNIFORM_COLOR")
        batch = gpu_extras.batch.batch_for_shader(shader, "LINES", vertices, indices=indices)
        shader.bind()
        shader.uniform_float("color", [1.0, 0.0, 0.0, 1.0])  # 赤色
        batch.draw(shader)

    def draw_callback():
        # シーン内の全Railオブジェクトに対して描画
        for obj in bpy.context.scene.objects:
            if obj.get("type") == "Rail":
                DrawRail.draw_rail_spline(obj)

    def register_draw():
        if DrawRail.handle is None:
            DrawRail.handle = bpy.types.SpaceView3D.draw_handler_add(
                DrawRail.draw_callback, (), 'WINDOW', 'POST_VIEW')

    def unregister_draw():
        if DrawRail.handle is not None:
            bpy.types.SpaceView3D.draw_handler_remove(DrawRail.handle, 'WINDOW')
            DrawRail.handle = None
