#include "studiox_includes.h"


scroll_wheel_service_provider::scroll_wheel_service_provider()
{
}

CString scroll_wheel_service_provider::GetVarDeclaration()
{
    return CString("GX_SCROLL_WHEEL_MEMBERS_DECLARE");
}

CString scroll_wheel_service_provider::DeclarePropertiesStruct()
{
    CString out(""
        "typedef struct\n"
        "{\n"
        "    int                 total_rows;\n"
        "    int                 selected_row;\n"
        "    GX_VALUE            row_height;\n"
        "    GX_UBYTE            start_alpha;\n"
        "    GX_UBYTE            end_alpha;\n"
        "    GX_RESOURCE_ID      wallpaper_id;\n"
        "    GX_RESOURCE_ID      selected_background;\n"
        "} GX_SCROLL_WHEEL_PROPERTIES;\n\n");
    return out;
}

CString scroll_wheel_service_provider::WriteExtendedProperties(screen_generator *gen, CString &prefix, widget_info *info)
{
    CString out;
    CString propname = prefix + info->app_name;

    out.Format(_T("GX_SCROLL_WHEEL_PROPERTIES %s_properties =\n")
        _T("{\n")
        _T("    %d, /* total rows */\n")
        _T("    %d, /* selected row */\n")
        _T("    %d, /* row height */\n")
        _T("    %d, /* start alpha */\n")
        _T("    %d, /* end alpha */\n")
        _T("    %s, /* wallpaper id */\n")
        _T("    %s  /* selected background */\n};\n"),
        propname,
        info->ewi.scroll_wheel.total_rows,
        info->ewi.scroll_wheel.selected_row,
        info->ewi.scroll_wheel.row_height,
        info->ewi.scroll_wheel.start_alpha,
        info->ewi.scroll_wheel.end_alpha,
        gen->GetPixelmapIdName(info->pixelmap_id[WALLPAPER_PIXELMAP_INDEX]),
        gen->GetPixelmapIdName(info->pixelmap_id[SCROLL_WHEEL_PIXELMAP_INDEX]));

    return out;
}


CString scroll_wheel_service_provider::GetCreateFromDefFunctionName()
{
    return CString("gx_studio_scroll_wheel_create");
}

CString scroll_wheel_service_provider::GetCreateFromDefFunction(int version)
{
    CString out;
    MakeCreatePreamble("scroll_wheel", version, out);

    out += "{\n"
        "    UINT status;\n"
        "    GX_SCROLL_WHEEL *wheel = (GX_SCROLL_WHEEL *) control_block;\n"
        "    GX_SCROLL_WHEEL_PROPERTIES *props = (GX_SCROLL_WHEEL_PROPERTIES *) info->properties;\n"
        "    status = gx_scroll_wheel_create(wheel, info->widget_name, parent, props->total_rows, \n"
        "                                    info->style, info->widget_id, &info->size);\n"
        "    if (status == GX_SUCCESS)\n"
        "    {\n"
        "        if (props->wallpaper_id)\n"
        "        {\n"
        "            gx_window_wallpaper_set((GX_WINDOW *) wheel, props->wallpaper_id, info->style & GX_STYLE_TILE_WALLPAPER);\n"
        "        }\n"
        "        if(props->selected_background)\n"
        "        {\n"
        "            gx_scroll_wheel_selected_background_set(wheel, props->selected_background);\n"
        "        }\n"
        "        gx_scroll_wheel_selected_set(wheel, props->selected_row);\n"
        "        gx_scroll_wheel_gradient_alpha_set(wheel, props->start_alpha, props->end_alpha);\n"
        "        gx_scroll_wheel_row_height_set(wheel, props->row_height);\n"
        "    }\n"
        "    return status;\n"
        "}\n";
    return out;
}


widget_info *scroll_wheel_service_provider::CreateNewInstance(GX_WIDGET *parent)
{
    int parent_size;

    GX_RECTANGLE size = parent->gx_widget_size;
    gx_utility_rectangle_define(&size, 0, 0, 79, 23);
    parent_size = (parent->gx_widget_size.gx_rectangle_right -
        parent->gx_widget_size.gx_rectangle_left) / 2;
    size.gx_rectangle_right = size.gx_rectangle_left + parent_size;

    parent_size = (parent->gx_widget_size.gx_rectangle_bottom -
        parent->gx_widget_size.gx_rectangle_top) / 2;
    size.gx_rectangle_bottom = size.gx_rectangle_top + parent_size;

    gx_utility_rectangle_center(&size, &parent->gx_widget_size);

    GX_SCROLL_WHEEL *wheel = new GX_SCROLL_WHEEL;
    gx_scroll_wheel_create(wheel, "scroll_wheel", parent, GX_NULL,
        GX_STYLE_ENABLED | GX_STYLE_BORDER_THIN | GX_STYLE_TEXT_CENTER, 0, &size);

    widget_info *info = InitWidgetInfo((GX_WIDGET *)wheel);
    info->pixelmap_id[WALLPAPER_PIXELMAP_INDEX] = 0;
    info->ewi.scroll_wheel.start_alpha = wheel->gx_scroll_wheel_gradient.gx_gradient_alpha_start;
    info->ewi.scroll_wheel.end_alpha = wheel->gx_scroll_wheel_gradient.gx_gradient_alpha_end;
    info->ewi.scroll_wheel.row_height = wheel->gx_scroll_wheel_row_height;
    info->ewi.scroll_wheel.selected_row = wheel->gx_scroll_wheel_selected_row;
    info->ewi.scroll_wheel.total_rows = 0;

    return info;
}

GX_WIDGET *scroll_wheel_service_provider::GenerateFromInfo(GX_WIDGET *parent, widget_info *info)
{
    GX_SCROLL_WHEEL *wheel = new GX_SCROLL_WHEEL;

    // Create scroll wheel widget.
    gx_scroll_wheel_create(wheel, (CHAR *)info->app_name.GetString(), parent,
        info->ewi.scroll_wheel.total_rows, info->style, 0, &info->size);

    // Set fill color.
    gx_widget_fill_color_set(wheel,
        info->color_id[NORMAL_FILL_COLOR_INDEX],
        info->color_id[SELECTED_FILL_COLOR_INDEX], 
        info->color_id[DISABLED_FILL_COLOR_INDEX]);

    // Set wallpaper.
    if (info->pixelmap_id[WALLPAPER_PIXELMAP_INDEX] > 0)
    {
        BOOL tile = FALSE;

        if (info->style & GX_STYLE_TILE_WALLPAPER)
        {
            tile = TRUE;
        }
        gx_window_wallpaper_set((GX_WINDOW *)wheel, info->pixelmap_id[WALLPAPER_PIXELMAP_INDEX], tile);
    }

    // Set selected row background.
    if (info->pixelmap_id[SCROLL_WHEEL_PIXELMAP_INDEX] > 0)
    {
        gx_scroll_wheel_selected_background_set((GX_SCROLL_WHEEL *)wheel, info->pixelmap_id[SCROLL_WHEEL_PIXELMAP_INDEX]);
    }

    // Set scroll wheel properties.
    gx_scroll_wheel_gradient_alpha_set(wheel, info->ewi.scroll_wheel.start_alpha, info->ewi.scroll_wheel.end_alpha);
    gx_scroll_wheel_row_height_set(wheel, info->ewi.scroll_wheel.row_height);
    gx_scroll_wheel_selected_set(wheel, info->ewi.scroll_wheel.selected_row);

    return (GX_WIDGET *)wheel;
}

void scroll_wheel_service_provider::SaveToProject(xml_writer &writer, studiox_project *project, int display, widget_info *info)
{
    window_service_provider::SaveToProject(writer, project, display, info);

    scroll_wheel_info *wheel_info = &info->ewi.scroll_wheel;

    WritePixelmapId(writer, project, display, "selected_background", info->pixelmap_id[SCROLL_WHEEL_PIXELMAP_INDEX]);
    writer.WriteInt("start_alpha", wheel_info->start_alpha);
    writer.WriteInt("end_alpha", wheel_info->end_alpha);
    writer.WriteInt("row_height", wheel_info->row_height);
    writer.WriteInt("selected_row", wheel_info->selected_row);
    writer.WriteInt("total_rows", wheel_info->total_rows);
}

void scroll_wheel_service_provider::ReadFromProject(xml_reader &reader, studiox_project *project, int display, widget_info *info, ULONG valid_styles)
{
    valid_styles |= GX_STYLE_WRAP;
    window_service_provider::ReadFromProject(reader, project, display, info, valid_styles);

    scroll_wheel_info *wheel_info = &info->ewi.scroll_wheel;

    info->pixelmap_id[SCROLL_WHEEL_PIXELMAP_INDEX] = ReadPixelmapId(reader, project, display, "selected_background");
    reader.ReadInt("start_alpha", wheel_info->start_alpha);
    reader.ReadInt("end_alpha", wheel_info->end_alpha);
    reader.ReadInt("row_height", wheel_info->row_height);
    reader.ReadInt("selected_row", wheel_info->selected_row);
    reader.ReadInt("total_rows", info->ewi.scroll_wheel.total_rows);
}


void scroll_wheel_service_provider::AssignScrollWheelInfo(widget_info *info, scroll_wheel_info *wheel_info)
{
    GX_SCROLL_WHEEL *wheel = (GX_SCROLL_WHEEL *) (info->widget);

    info->ewi.scroll_wheel = *wheel_info;

    if (wheel)
    {
        gx_scroll_wheel_gradient_alpha_set(wheel, wheel_info->start_alpha, wheel_info->end_alpha);
        gx_scroll_wheel_row_height_set(wheel, wheel_info->row_height);
    }

    if (wheel_info->total_rows > 0)
    {
        if (wheel)
        {
            gx_scroll_wheel_total_rows_set(wheel, wheel_info->total_rows);
        }
    }
    else
    {
        if (info->basetype == GX_TYPE_NUMERIC_SCROLL_WHEEL)
        {
            if (wheel)
            {
                GX_NUMERIC_SCROLL_WHEEL *nw = (GX_NUMERIC_SCROLL_WHEEL *) wheel;
                gx_numeric_scroll_wheel_range_set(nw, nw->gx_numeric_scroll_wheel_start_val, nw->gx_numeric_scroll_wheel_end_val);
            }
        }
    }

    if (wheel)
    {
        gx_scroll_wheel_selected_set(wheel, wheel_info->selected_row);
    }
}

void scroll_wheel_service_provider::AssignPixelmap(widget_info* info, int index, GX_RESOURCE_ID pixmap_id)
{
    GX_WIDGET* widget = info->widget;

    switch (index)
    {
    case SCROLL_WHEEL_PIXELMAP_INDEX:
        if (widget)
        {
            gx_scroll_wheel_selected_background_set((GX_SCROLL_WHEEL*)widget, pixmap_id);
        }

        info->pixelmap_id[SCROLL_WHEEL_PIXELMAP_INDEX] = pixmap_id;
        break;

    default:
        window_service_provider::AssignPixelmap(info, index, pixmap_id);
    }
}