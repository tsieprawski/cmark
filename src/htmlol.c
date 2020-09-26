#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "cmark_ctype.h"
#include "config.h"
#include "cmark.h"
#include "node.h"
#include "buffer.h"
#include "houdini.h"
#include "scanners.h"

#define BUFFER_SIZE 100

// Functions to convert cmark_nodes to HTML strings.

static int escape_html(cmark_strbuf *dest, const unsigned char *source,
                        bufsize_t length) {
  int ret = 1;
  for (unsigned int i = 0; i < length; i++) {
  	if ('\n' == dest->ptr[i] || '\r' == dest->ptr[i]) {
      ret = 1;
      length = i;
      break;
  	}
  }
                        
  houdini_escape_html0(dest, source, length, 0);
  return ret;
}

struct render_state {
  cmark_strbuf *html;
  cmark_node *plain;
};

static int S_render_node(cmark_node *node, cmark_event_type ev_type,
                         struct render_state *state, int options) {
  cmark_strbuf *html = state->html;

  bool entering = (ev_type == CMARK_EVENT_ENTER);

  if (state->plain == node) { // back at original node
    state->plain = NULL;
  }

  if (state->plain != NULL) {
    switch (node->type) {
    case CMARK_NODE_TEXT:
    case CMARK_NODE_CODE:
    case CMARK_NODE_HTML_INLINE:
      return escape_html(html, node->data, node->len);

    case CMARK_NODE_LINEBREAK:
    case CMARK_NODE_SOFTBREAK:
      cmark_strbuf_putc(html, ' ');
      break;

    default:
      break;
    }
    return 1;
  }

  switch (node->type) {
  case CMARK_NODE_DOCUMENT:
  case CMARK_NODE_PARAGRAPH:
    break;

  case CMARK_NODE_TEXT:
    return escape_html(html, node->data, node->len);

  case CMARK_NODE_STRONG:
    if (entering) {
      cmark_strbuf_puts(html, "<strong>");
    } else {
      cmark_strbuf_puts(html, "</strong>");
    }
    break;

  case CMARK_NODE_EMPH:
    if (entering) {
      cmark_strbuf_puts(html, "<em>");
    } else {
      cmark_strbuf_puts(html, "</em>");
    }
    break;

  case CMARK_NODE_LINK:
    if (entering) {
      int ret = 1;
      cmark_strbuf_puts(html, "<a href=\"");
      if (node->as.link.url && ((options & CMARK_OPT_UNSAFE) ||
                                !(_scan_dangerous_url(node->as.link.url)))) {
        houdini_escape_href(html, node->as.link.url,
                            strlen((char *)node->as.link.url));
      }
      if (node->as.link.title) {
        cmark_strbuf_puts(html, "\" title=\"");
        ret = escape_html(html, node->as.link.title,
                    strlen((char *)node->as.link.title));
      }
      cmark_strbuf_puts(html, "\">");
      return ret;
    } else {
      cmark_strbuf_puts(html, "</a>");
    }
    break;

  default:
    return 0;
  }

  return 1;
}

char *cmark_render_htmlol(cmark_node *root, int options) {
  char *result;
  cmark_strbuf html = CMARK_BUF_INIT(root->mem);
  cmark_event_type ev_type;
  cmark_node *cur;
  struct render_state state = {&html, NULL};
  cmark_iter *iter = cmark_iter_new(root);

  while ((ev_type = cmark_iter_next(iter)) != CMARK_EVENT_DONE) {
    cur = cmark_iter_get_node(iter);
    if (0 == S_render_node(cur, ev_type, &state, options)) {
      break;
    }
  }
  result = (char *)cmark_strbuf_detach(&html);

  cmark_iter_free(iter);
  return result;
}
