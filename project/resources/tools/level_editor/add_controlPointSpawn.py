import bpy

class MYADDON_OT_add_control_point_spawn(bpy.types.Operator):
    bl_idname = "myaddon.myaddon_ot_add_control_point_spawn"
    bl_label = "ControlPointSpawn追加"
    bl_description = "RailにControlPointSpawnを追加します"
    bl_options = {'REGISTER', 'UNDO'}

    def execute(self, context):
        parent_obj = context.active_object
        if not parent_obj or parent_obj.get("type") != "Rail":
            self.report({'WARNING'}, "Railオブジェクトを選択してください")
            return {'CANCELLED'}

        # ControlPointSpawn作成 (既存のオペレータを使用)
        bpy.ops.myaddon.myaddon_ot_spawn_create_controlpoint_symbol('EXEC_DEFAULT')
        child_obj = context.active_object

        # 親子付け
        child_obj.parent = parent_obj
        # 位置をリセット (親の原点に配置されるので、少しずらすなどしたほうがいいかもしれないが、一旦原点)
        child_obj.location = (0, 0, 0)
        
        # 親を選択状態に戻す（連続して追加しやすくするため）
        bpy.ops.object.select_all(action='DESELECT')
        parent_obj.select_set(True)
        context.view_layer.objects.active = parent_obj
        
        return {'FINISHED'}
