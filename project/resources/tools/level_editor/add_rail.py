import bpy

class MYADDON_OT_add_rail(bpy.types.Operator):
    bl_idname = "myaddon.myaddon_ot_add_rail"
    bl_label = "Rail追加"
    bl_description = "レールを追加します"
    bl_options = {'REGISTER', 'UNDO'}

    def execute(self, context):
        parent_obj = context.active_object
        
        # 親オブジェクトが選択されていない場合はエラー (active_objectは選択解除後も残る場合があるためselect_getで確認)
        if not parent_obj or not parent_obj.select_get():
            self.report({'WARNING'}, "親となるオブジェクトを選択してください")
            return {'CANCELLED'}
        
        # Emptyオブジェクト作成
        bpy.ops.object.empty_add(type='PLAIN_AXES', radius=1.0)
        rail_obj = context.active_object
        rail_obj.name = "Rail"
        rail_obj["type"] = "Rail"
        
        # 親子付けを行う
        rail_obj.parent = parent_obj
        rail_obj.location = (0, 0, 0)
        
        # 親を選択状態に戻す
        bpy.ops.object.select_all(action='DESELECT')
        parent_obj.select_set(True)
        context.view_layer.objects.active = parent_obj
        
        return {'FINISHED'}

class OBJECT_PT_rail(bpy.types.Panel):
    bl_idname = "OBJECT_PT_rail"
    bl_label = "Rail Object"
    bl_space_type = 'PROPERTIES'
    bl_region_type = 'WINDOW'
    bl_context = "object"

    @classmethod
    def poll(cls, context):
        return context.object and context.object.get("type") == "Rail"

    def draw(self, context):
        layout = self.layout
        # ここにコントロールポイント追加ボタンなどを配置予定
        # importされている前提でIDを指定
        layout.operator("myaddon.myaddon_ot_add_control_point_spawn")
